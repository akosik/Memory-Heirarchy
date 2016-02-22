import sys
import subprocess
import numpy as np

def run(size,sizemax,f):

    if size <= sizemax:
        times = []

        for j in range(1,10):
            time = int(subprocess.check_output(["./mem",str(size)]))
            times.append(time)

        avg = np.mean(times)
        f.write(str(size) + " " + str(avg) + " \n")

        run(size*2,sizemax,f)

if __name__ == "__main__":


    if sys.argc != 3:
        print("Invalid usage.\nUsage: python(3) runtests.py <sizemin> <sizemax>")
        exit(-1)
    sizemin = int(sys.argv[1])
    sizemax =int( sys.argv[2])
    size = sizemin
    with open("tests.dat","w+") as f:
        f.write("# size time \n")
        run(size,sizemax,f)
    exit(0)
