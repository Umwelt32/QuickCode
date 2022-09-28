from ursina import *
import numpy,cv2,math
from PIL import Image

def texture_to_height_values(path, skip=1,fc=0.5):
    heightmap = cv2.imread(path,cv2.IMREAD_GRAYSCALE)
    skip = math.pow(skip,2)    # should be power of two. only works with heightmap, not height_values.
    width = math.floor(heightmap.shape[0]//skip)
    depth = math.floor(heightmap.shape[1]//skip)
    hi_val = int(math.sqrt(width*depth))
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
    if val < 50:
        RGB=(10,10,200)
    elif val < 80:
        RGB=(64,64,220)
    elif val < 110:
        RGB=(200,150,50)
    elif val < 150:
        RGB=(200,200,50)
    elif val < 180:
        RGB=(100,200,50)
    elif val < 220:
        RGB=(50,140,20)
    elif val < 240:
        RGB=(240,250,240)
    else:
        RGB=(255,255,255)
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
    Tx = texture_to_height_values(sys.argv[1], 10)
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
