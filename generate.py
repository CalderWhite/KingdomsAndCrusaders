"""Mass generate gifs on every core."""
# Note: This will likely be IO bound.

import os
import sys
from multiprocessing import Pool
import subprocess


OUT_DIR = "/tmp/dist"


def worker(name):
    #./bazel-bin/video /tmp/out.gif 600 300 7 8 2 3000
    subprocess.run(["./bazel-bin/video", name, "600", "300", "7", "8", "2", "3000"])


def main(args):
    num_gifs = int(args[0])
    if not os.path.exists(OUT_DIR):
        os.mkdir(OUT_DIR)
    names = [f"{OUT_DIR}/{i}.gif" for i in range(num_gifs)]
    p = Pool()
    p.map(worker, names)


if __name__ == '__main__':
    main(sys.argv[1:])