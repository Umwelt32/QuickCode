from ursina import *
import numpy,cv2,math

def load_hmap_float(path):
    img = cv2.imread(path,cv2.IMREAD_GRAYSCALE)
    norm = cv2.normalize(img, None, 0, 255, cv2.NORM_MINMAX)
    num_array = numpy.asarray(norm)
    y_shape = int(math.sqrt(img.shape[0]*img.shape[1]))
    num_array_fl=(num_array/255.0)
    col_map=load_color_texture(num_array_fl)
    return [numpy.asarray(num_array_fl).tolist(),(img.shape[0],y_shape,img.shape[1])]

def load_color_texture(arr):
    print('stub')
    return arr

if __name__ == '__main__':
    app = Ursina()
    hv = load_hmap_float('N54E023_hgt_mdbu.bmp')
    print(str(hv[1]))
    terrain_from_list = Entity(model=Terrain(height_values=hv[0], scale=hv[1], texture='heightmap_1', x=512))
    def input(key):
        if key == 'space':  # randomize the terrain
            print('stub')
    EditorCamera()
    Sky()
    def update():
        print('stub')
    app.run()