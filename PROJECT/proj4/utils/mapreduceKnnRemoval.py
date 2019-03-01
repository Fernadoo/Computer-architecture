import numpy as np
import sys
from pyspark import SparkContext
from functools import reduce

sc = SparkContext()

def knnRemoval(image, k, kernelSize, distance_thr):
    filtered_image = np.zeros(image.shape, dtype=image.dtype)
    cnt = np.zeros((1))
    ###
    halfSize = int(kernelSize/2)
    width = image.shape[1]
    height = image.shape[0]
    angle_width_per_pixel = np.pi*2/image.shape[1]
    angle_height_per_pixel = np.pi*2/image.shape[0]
    x_grid, y_grid = np.meshgrid(list(range(kernelSize)), list(range(kernelSize)))
    def knnRemovalValue(image, y, x,  k, kernelSize, distance_thr):
        center_depth = image[y,x]
        depth_squared = center_depth**2
        width_per_pixel = np.sqrt( 2*depth_squared - (np.cos(angle_width_per_pixel)*2*depth_squared) )
        height_per_pixel = np.sqrt( 2*depth_squared - (np.cos(angle_height_per_pixel)*2*depth_squared) )
        kernel = np.abs(image[y-halfSize:y+halfSize+1, x-halfSize:x+halfSize+1] - center_depth)\
                 + np.abs(width_per_pixel * (x_grid-halfSize))\
                + np.abs(height_per_pixel * (y_grid-halfSize))
        # # sorted_kernel = np.sort(kernel.reshape(-1))
        # sorted_kernel = np.partition(kernel.reshape(-1),k)
        # sorted_kernel = sorted_kernel[np.where(sorted_kernel!=0)]

        kernel_ = kernel.reshape(-1)
        kernel_not_0 = kernel_[np.where(kernel_!=0)]
        lessthan = kernel_not_0[np.where(kernel_not_0 < distance_thr)]
        if lessthan.size >= k:
            return (image[y,x],0)
        sorted_kernel = np.partition(kernel_not_0,k)

        mean = np.mean(sorted_kernel[:k])#a little bit different from cpp version here
        if(mean > distance_thr):
            cnt[0] += 1
            return (0.,1)
        return (image[y,x],0)

    # for i in range(image.shape[0]):
    #     for j in range(image.shape[1]):
    #         if(i >= halfSize and j >= halfSize and i < image.shape[0] - halfSize and j < image.shape[1] - halfSize):
    #             if(image[i,j] != 0):
    #                 filtered_image[i,j] = knnRemovalValue(image, i, j, k, kernelSize, distance_thr)
    #     sys.stdout.write("\rrow %d/%d of image"%(i, image.shape[0]))
    #     sys.stdout.flush()
    # sys.stdout.write("\n")

    def map_func(location):
        if(location[0] >= halfSize and location[1] >= halfSize and location[0] < height-halfSize and  location[1] < width-halfSize):
            if (image[location[0],location[1]] != 0):
                # tmp = cnt[0]
                return knnRemovalValue(image,location[0],location[1],k,kernelSize,distance_thr)
                # if cnt[0] > tmp:
                #     return 1
        return (0.,0)


    imagelist = []
    for i in range(height):
        for j in range(width):
            imagelist.append([i,j])
    RDD = sc.parallelize(imagelist)
    RDD_ = RDD.map(map_func).collect()

    filtered_image = np.array(list(map(lambda x: x[0], RDD_))).reshape(height,width)
    cnt[0] = reduce(lambda x,y: x+y,list(map(lambda x: x[1], RDD_)))


    #TODO
    ###
    return filtered_image, cnt[0]
