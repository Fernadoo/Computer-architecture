from pyspark import SparkContext
sc = SparkContext("local","wordCount")
text_RDD = sc.textFile("test.html")#textFile does not support url, please download http://shtech.org/course/ca/18s/ locally
counts_RDD = text_RDD.flatMap(lambda x: x.split(" ")).map(lambda x: (x, 1)).reduceByKey(lambda x,y: x+y)
print(counts_RDD.take(1)[0][1])
