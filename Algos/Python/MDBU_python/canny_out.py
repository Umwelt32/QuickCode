##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## Convert canny detector samples
## References:
##############
##############
##################################################
import cv2,sys,os

def doCanny(img,t1,t2,kernel,inv):
    edges = cv2.Canny(img,t1,t2,kernel)
    masked = cv2.bitwise_and(img, img, mask=edges)
    if inv==True:
        masked = cv2.bitwise_not(masked)
    return masked

def canny_output(fileName,outf):
    img = cv2.imread(fileName)
    out = doCanny(img,32,64,15,False)
    cv2.imwrite(outf, out)


if __name__ == "__main__":
    fileName=str(sys.argv[1])
    print('Open File: '+fileName+'...')
    canny_output(fileName,fileName.replace('.','_')+'_out.bmp')
    exit(0)

