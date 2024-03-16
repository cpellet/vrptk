class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def xassert(item, name):
    if item:
        print(f"{bcolors.OKGREEN}✅ {name}{bcolors.ENDC}")
    else:
        print(f"{bcolors.FAIL}❌ {name}{bcolors.ENDC}")
        exit(1)

def plog(text):
    print(f"{bcolors.OKCYAN}{text}{bcolors.ENDC}")