##################################################
## Author: https://github.com/Umwelt32/QuickCode
## Copyright: 2022
## References:
##############
##############
##################################################

from ursina import *
import numpy,cv2,math

def texture_to_height_values(path, skip=1,fc=0.5,hf=2.0):
    heightmap = cv2.imread(path,cv2.IMREAD_GRAYSCALE)
    skip = math.pow(skip,2)    # should be power of two. only works with heightmap, not height_values.
    width = math.floor(heightmap.shape[0]//skip)
    depth = math.floor(heightmap.shape[1]//skip)
    hi_val = int(math.sqrt(width*depth))*hf
    height_values = cv2.normalize(heightmap, None, 0, 255, cv2.NORM_MINMAX)
    genColorTexture(height_values)
    height_values = (numpy.asarray(height_values)/255.0)+fc
    height_values = numpy.power(height_values,2)
    height_values = numpy.flip(height_values, axis=0)
    height_values = numpy.swapaxes(height_values, 0, 1)
    height_values=height_values*hi_val
    return [height_values.tolist(),(width,hi_val,depth)]

def getRgbByGray(val):
    RGB = (0,0,0)
    color_map = [[50,[50,50,255]], [75,[100,100,200]], [100,[200,150,50]], [120,[200,150,0]], [160,[0,150,50]], [200,[0,64,10]],[220,[255,255,200]],[240,[255,255,255]]]
    for color in color_map:
        if int(val) <= int(color[0]):
            RGB=(color[1][0],color[1][1],color[1][2])
            break
    BGR = (RGB[2],RGB[1],RGB[0])
    return BGR

def genColorTexture(gray_img):
    width,height=gray_img.shape[0],gray_img.shape[1]
    blank_image = numpy.zeros((height,width,3), numpy.uint8)
    for x in range(width):
        for y in range(height):
            blank_image[x][y]=getRgbByGray(gray_img[x][y])
    cv2.imwrite('texture_out.bmp', blank_image)
    tex = Texture('texture_out.bmp')
    return tex;

if __name__ == '__main__':
    app = Ursina()
    Tx = texture_to_height_values(sys.argv[1], 12)
    hv = Tx[0]
    EditorCamera()
    Sky()
    _terrain = Entity(model=Terrain(height_values=hv), scale=Tx[1], texture='texture_out.bmp')
    _terrain.position=(0,0,0)
    camera.position=_terrain.position
    camera.position.y=-Tx[1][1]*2
    camera.look_at(_terrain, axis='forward') 
    def input(key):
        if key == 'space':  # randomize the terrain
            print('stub')
    def update():
        print('stub')
    app.run()
