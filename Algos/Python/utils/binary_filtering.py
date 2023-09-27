##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## binary filtering
## References:
##############
##############
##################################################

import cv2,numpy,math,sys

def _load_img_grayscale(path):
    return cv2.cvtColor(cv2.imread(str(path)), cv2.COLOR_BGR2GRAY)

def _img_binary_filtering(img,t):
    _max = int(numpy.max(img))
    _tv  = int(math.floor(float(_max)*t))
    ret,_img = cv2.threshold(img,_tv,_max,cv2.THRESH_BINARY)
    _img[_img!=0]=255
    _img=255-_img
    return _img

def _img_binary_filtering_path(path,t):
    return _img_binary_filtering(_load_img_grayscale(path),t)

def _img_save(path,img):
    cv2.imwrite(path,img)
    
if __name__ == "__main__":
    _img_save('output.bmp',_img_binary_filtering_path(str(sys.argv[1]),0.75))
    