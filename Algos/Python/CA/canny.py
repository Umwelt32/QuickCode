##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2023
## canny
## References:
##############
##############
##################################################
import os,sys,math,numpy,random,cv2


if __name__ == "__main__":
    image   = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
    blurred = cv2.GaussianBlur(image, (5, 5), 0)
    edges   = cv2.Canny(blurred, 50, 150)
    dilated_image = cv2.dilate(edges, numpy.ones((2, 2), numpy.uint8), iterations=2)
    cv2.imwrite('canny.bmp', dilated_image)
    exit(0)
