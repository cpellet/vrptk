import random
import math
import csv
import os
import sys
from cy import xassert, plog
from argparse import ArgumentParser
import xml.etree.ElementTree as ET 

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--dataset", type=str, required=True)
    parser.add_argument("--out", type=str, required=True)
    parser.add_argument("--cs_number", type=int, default=1)
    parser.add_argument("--cs_capacity", type=int, default=1)
    parser.add_argument("--cs_power", type=int, default=11)
    parser.add_argument("--price_profile", type=str, default="france", choices=["france", "italy", "belgium", "spain", "uk", "germany"])
    parser.add_argument("--veh_capacity", type=int, default=40) # kwh
    parser.add_argument("--veh_soc_mean", type=int, default=90)
    parser.add_argument("--veh_soc_std", type=int, default=5)
    parser.add_argument("--veh_min_soc", type=int, default=20)
    parser.add_argument("--veh_max_soc", type=int, default=99)
    parser.add_argument("--veh_consumption", type=float, default=0.2)
    parser.add_argument("--veh_fc", type=float, default=0.1)
    parser.add_argument("--veh_vc", type=float, default=0.1)
    parser.add_argument("--seed", type=int, default=42)
    args = parser.parse_args()
    random.seed(args.seed)
    plog(f'Will generate {args.cs_number} charging stations with `{args.price_profile}` prices for dataset {args.dataset}')
    dataset = ET.parse(args.dataset)
    root = dataset.getroot()
    nodes = root.find("network/nodes")
    coords_ok, depot_ok = True, False
    depot_id, tw_s, tw_e = None, math.inf, -math.inf
    for node in nodes.findall("node"):
        coords_ok = coords_ok and node.find("cx") != None and node.find("cy") != None
        if node.get("type") == "0":
            depot_ok = True
            depot_id = node.get("id")
    xassert(coords_ok, "All nodes have coordinates")
    xassert(depot_ok, "At least one depot")
    requests = root.find("requests")
    demand_ok, time_ok, interval_ok = True, True, True
    for request in requests.findall("request"):
        demand_ok = demand_ok and request.find("quantity") != None
        time_ok = time_ok and request.find("service_time") != None
        tw_s = min(tw_s, int(request.find("tw/start").text))
        tw_e = max(tw_e, int(request.find("tw/end").text))
        interval_ok = interval_ok and request.find("tw/start") != None and request.find("tw/end") != None
    xassert(demand_ok, "All requests have demand")
    xassert(time_ok, "All requests have service time")
    xassert(interval_ok, "All requests have time windows")
    fleet = root.find("fleet")
    n_vehicle_ok, depot_match_ok, capacity_ok = True, True, True
    for vehicle in fleet.findall("vehicle_profile"):
        n_vehicle_ok = n_vehicle_ok and vehicle.get("number") != None
        depot_match_ok = depot_match_ok and vehicle.find("departure_node").text == depot_id and vehicle.find("arrival_node").text == depot_id
        capacity_ok = capacity_ok and vehicle.find("capacity") != None
        energy_capacity = ET.SubElement(vehicle, "energy_capacity")
        energy_capacity.text = str(args.veh_capacity)
        i_soc = ET.SubElement(vehicle, "soc")
        ET.SubElement(i_soc, "min").text = str(args.veh_min_soc / 100)
        ET.SubElement(i_soc, "max").text = str(args.veh_max_soc / 100)
        ET.SubElement(i_soc, "initial").text = str(round(random.gauss(args.veh_soc_mean, args.veh_soc_std) / 100, 3))
        ET.SubElement(vehicle, "consumption").text = str(args.veh_consumption)
        ET.SubElement(vehicle, "fixed_cost").text = str(args.veh_fc)
        ET.SubElement(vehicle, "variable_cost").text = str(args.veh_vc)
    xassert(n_vehicle_ok, "All profiles have a count")
    xassert(depot_match_ok, "All profiles have the depot as departure and arrival")
    xassert(capacity_ok, "All profiles have a capacity")
    plog("Dataset is valid!")
    xassert(True, "Vehicle profiles updated successfully")
    station_nodes = random.sample(list(nodes), args.cs_number)
    for station in station_nodes:
        station.set("type", "2")
    stations = ET.Element("stations")
    s_id = 0
    for station in station_nodes:
        st = ET.SubElement(stations, "station", {"id": str(s_id), "node": station.get("id")})
        ET.SubElement(st, "capacity").text = str(args.cs_capacity)
        ET.SubElement(st, "power").text = str(args.cs_power)
        s_id += 1
    for req in requests.findall("request"):
        if req.get("id") in [station.get("id") for station in station_nodes]:
            requests.remove(req)
    root.append(stations)
    xassert(len(station_nodes) == args.cs_number, "Stations generated successfully")
    info = root.find("info")
    ET.SubElement(info, "patch").text = "EV-VRPTW / Cyrus Pellet"
    i_tw = ET.SubElement(info, "tw")
    tw_s, tw_e = round(tw_s, -3), round(tw_e, -3)
    ET.SubElement(i_tw, "start").text = str(tw_s)
    ET.SubElement(i_tw, "end").text = str(tw_e)
    prices = ET.SubElement(root, "prices")
    with open(f'{sys.path[0]}/elec_prices.csv', 'r') as pfile:
        reader = csv.reader(pfile)
        header = next(reader)
        index = header.index(args.price_profile)
        days = set()
        for row in reader:
            days.add(row[0])
        day = random.choice(list(days))
        plog(f"Using prices for day {day}")
        pfile.seek(0)
        idx = 0
        for row in reader:
            if row[0] == day:
                price = ET.SubElement(prices, "point", {"start": str(round(tw_e / 24 * idx)), "end": str(round(tw_e / 24 * (idx + 1)))})
                ET.SubElement(price, "value").text = str(round(float(row[index]) / 1000, 3))
                idx += 1
    xassert(prices != None, "Prices generated successfully")
    ET.indent(dataset, space="\t", level=0)
    dataset.write(args.out)
