#!/usr/bin/python

def write(words):
    with open("words.txt", "w") as w:
        w.truncate()
        for word in words:
            w.write(word)
            w.write("\n")

def read ():
    with open("words.txt", "r") as w:
        result = []
        for line in w:
            result.append(line.strip("\n")) 
    return result
        

if __name__ == "__main__":
    while (True):
        words = input("Anna kolme sanaa: ").split(" ")
        if len( words) == 3:
            break 
    

    write(words)
    print(read())
    
