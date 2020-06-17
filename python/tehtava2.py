#!/usr/bin/python


def factorial(x):
    if x== 0:
        return 1
    return x * factorial(x-1)

if __name__ == "__main__":
    number = int(input("Anna luku: "))
    print("Kertoma on {:d}".format(factorial(number)))
