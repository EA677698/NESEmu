with (open("../latestLog.txt", "r") as log):
    lines = log.read().split("\n")
    complete_view = []
    temp = []
    for x in lines:
        index = x.find("Tile Index")
        if index == -1:
            continue
        index = index + 12
        if len(temp) > 32:
            complete_view.append(temp)
            temp = []
        temp.append(x[index:].strip())
    complete_view.append(temp)
    print(complete_view)

with open("view.csv", "w") as out:
    for x in complete_view:
        str = ""
        for y in x:
            str += y + ","
        out.write(str[0:len(str) - 2] + "\n")