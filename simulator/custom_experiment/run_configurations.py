import sys
import subprocess
from multiprocessing.pool import ThreadPool

conf_file = sys.argv[0]
output_folder = sys.argv[1]

def run_configurations(cmd):
    try:
        run_configurations.counter += 1
    except AttributeError:
        run_configurations.counter = 1
    
    with open(f'{output_folder}/out_{run_configurations.counter}.txt', "w") as f:
        subprocess.run([cmd, '1', conf_file], stdout = f)
    
    return

cmds = ['../simulator']*100

with ThreadPool(12) as pool:
    pool.map(run_configurations, cmds)


