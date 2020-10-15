#! /usr/bin/env python
# coding=utf-8
import pty
import os
import select
import serial


def mkpty():
    master, slave = pty.openpty()
    slaveName = os.ttyname(slave)
    print('\nslave names: ', slaveName)
    return master


if __name__ == "__main__":

    master = mkpty()
    while True:
        r, w, ex = select.select([master], [], [], 0.5)
        for masters in r:
            data = os.read(masters, 128)
            print("read %d data." % len(data), data)
