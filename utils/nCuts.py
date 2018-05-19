from skimage import data, segmentation, color, io
import scipy.misc
from skimage.future import graph
from matplotlib import pyplot as plt


img = io.imread('1.jpg')

labels1 = segmentation.slic(img, compactness=30, n_segments=800)
out1 = color.label2rgb(labels1, img, kind='avg')

g = graph.rag_mean_color(img, labels1, mode='similarity')
labels2 = graph.cut_normalized(labels1, g)
out2 = color.label2rgb(labels2, img, kind='avg')



scipy.misc.imsave("result.png",out2)


print 'done'
