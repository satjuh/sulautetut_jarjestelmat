#!/usr/bin/python


if __name__ == "__main__":
    first = int(input("Anna ensimmaine luku:"))

    if first < 50:
        print("{:d} oli pienempi kuin 50".format(first))
    else:
        print("{:d} oli suurepi kuin 50".format(first))

    second = int(input("Anna toinen luku: "))

    print("Tulos on: \n {:d}".format(first+ second))