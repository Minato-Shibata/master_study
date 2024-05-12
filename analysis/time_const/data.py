import pandas as pd
import glob
import os
import fnmatch
import math
import csv
import io

adf = glob.glob("*.CSV")
li = []
for filename in adf:
    df = pd.read_csv(filename, names=("0","1","2","3","4","5"))
    df3 = df.set_index("3")
    df2 = df3.iloc[50:2250]
    df1 = df2.drop(df.columns[[0,1,2,5]], axis=1)
    li.append(df1)

df1 = pd.concat(li, axis=1, ignore_index=True)
#print(df1)

path = os.getcwd()  
files = os.listdir(path)
csv = fnmatch.filter(files,"*.CSV")
count = len(csv)  
#print(count)

df_pr = df1.apply(lambda li:li/sum(abs(li)/count))

#print(df_pr)
#result = df_pr.sum(axis=1)
result = df_pr.T.describe()
#print(result)
result_mean = df_pr.T.mean()
result_sum = df_pr.T.sum()
result_cov = df_pr.T.cov()
result_std = df_pr.T.std()

#print(result_mean, result_sum, result_std)
#result = result_mean.join(result_std)
result = pd.concat([result_mean,result_std/math.sqrt(count)],axis=1)
#print(result_mean, result_sum, result_cov, result_std)
#print(result)
#result.to_csv("result2.dat", header=None, sep=" ")


result.insert(1, '', 0)
# print(result)
result.to_csv("result2.dat", header=None, sep=" ")

