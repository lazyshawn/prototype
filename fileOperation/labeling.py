import os
import csv
import re

cur_dir = './04/'

# 打标签
def labeling(mp_index):
    label, index, context = [], [], []
    fold_dir = os.path.join(cur_dir, "label/")
    with open(cur_dir+mp_index+'-label.txt',"r") as f:
        for line in f.readlines():
            # 去除空行
            line = re.sub('[\n]', '', line)
            # 正则匹配
            line = re.match(r"([0-9]+),\s+([0-9]+)", str(line))
            if line:
                index.append(line.group(1))
                label.append(line.group(2))

    with open(cur_dir+mp_index+'.csv',"r") as f:
        i = 0
        for line in f.readlines()[1:]:
            line = re.sub('[\n]+', '', line)
            if (int(line.split(",")[0]) > (int(index[i]))):
                i += 1
            context.append([line.split(',')[0],line.split(',')[1], str(label[i])])
            
    #  print(context)

    if os.path.exists(fold_dir):
        print("Begin to write:")
    else:
        os.mkdir(cur_dir+"label")

    # 创建文件
    file = open(fold_dir+mp_index+'.csv','w')
    file.close()
    # 写入文件
    with open(fold_dir+mp_index+'.csv','w') as csvfile:
            writer = csv.writer(csvfile)
            #写入表头
            writer.writerow(['sequence','timestamp', 'class'])
            #将数据写入csv文件
            writer.writerows(context)

# 读取文件
def readfile(fname):
    with open(fname, 'r') as f:
        for line in f.readlines():
            print(line)

if __name__ == "__main__":
    maplist = [mp for mp in os.listdir(cur_dir) if mp[-10:]=='-label.txt']
    #  print(maplist)
    for mp in maplist:
        labeling(mp[0:-10])

