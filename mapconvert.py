import json
import argparse

parser = argparse.ArgumentParser(description='A tool for converting from Tiled\'s .json format to .map files')
parser.add_argument('-i', required=True, help='The path to the input .json file')
parser.add_argument('-r', required=True, help='The path to the resulting .map file')
parser.add_argument('-n', required=True, help='The name of the map')
args = parser.parse_args()
print(args)

map_name = args.n
result_path = args.r
in_path = args.i #'asset/test_map.json'

# load the json file
f = open(in_path)
j = json.load(f)

# check that there are two layers 'Ground' and 'Entities' and that their sizes are equal
dimxg = -1; dimyg = -1
dimxe = -1; dimye = -1
dimxd = -1; dimyd = -1
has_ground = False 
has_entities = False
has_decoration = False
ground_idx = -1
entity_idx = -1
decor_idx = -1
object_idx = -1
for i,l in enumerate(j['layers']):
    #print(l['name'])
    if (l['name'] == 'Ground'):
        has_ground = True
        dimxg = l['width']
        dimyg = l['height']
        ground_idx = i
    elif (l['name'] == 'Entities'):
        has_entities = True
        dimxe = l['width']
        dimye = l['height']
        entity_idx = i
    elif (l['name'] == 'Object'):
        object_idx = i
    elif (l['name'] == 'Decoration'):
        has_decoration = True
        dimxd = l['width']
        dimyd = l['height']
        decor_idx = i
        
if (not has_ground):
    print('MISSING GROUND LAYER')
elif (not has_entities):
    print('MISSING ENTITY LAYER')
elif (dimxg != dimxe or dimyg != dimye):
    print('LAYER SIZE MISMATCH')
else:
    print('MAP VALID: dimx=' + str(dimxg) + ' dimy=' + str(dimyg))
	
# find the 'startx' and 'starty' properties of 'Ground' 
_sd = j['layers'][ground_idx]['properties']
start_dat = {}
for p in _sd:
    start_dat[p['name']] = p['value']
	
# open file
r = open(result_path, 'w')
# write name and map dim to file
r.write('NAME;' + str(map_name)+'\n')
r.write('MAPDIM;'+str(dimxg)+','+str(dimyg)+'\n')
# write map data
r.write('TILES;'+str(j['layers'][ground_idx]['data']).replace(' ','')[1:-1]+'\n')
r.write('ENTITIES;'+str(j['layers'][entity_idx]['data']).replace(' ','')[1:-1]+'\n')
if (has_decoration):
    r.write('DECORATION;'+str(j['layers'][decor_idx]['data']).replace(' ','')[1:-1]+'\n')
# write start info
r.write('START;'+str(start_dat['startx'])+','+str(start_dat['starty'])+'\n')
# write portal info
for o in j['layers'][object_idx]['objects']:
	if (o['type'] == 'PORTAL'):
		if ('properties' in o):
			odict = {}
			for p in o['properties']:
				odict[p['name']] = p['value']
			if 'end_tutorial' in odict:
				r.write(o['type']+str(';')+str(o['x'])+','+str(o['y'])+','+o['name']+',1\n')
				continue
		r.write(o['type']+str(';')+str(o['x'])+','+str(o['y'])+','+o['name']+',0\n')
	elif (o['type'] == 'DIALOGUE'):
		odict = {}
		for p in o['properties']:
			odict[p['name']] = p['value']
		r.write(o['type']+str(';')+str(o['x'])+','+str(o['y'])+','+o['name']+','+odict['Count']+','+odict['Dialogue']+'\n')
	elif (o['type'] == 'DOOR'):
		odict = {}
		for p in o['properties']:
			odict[p['name']] = p['value']
		r.write(o['type']+str(';')+str(o['x'])+','+str(o['y'])+','+o['name']+','+odict['targetx']+','+odict['targety']+'\n')
# close
r.close()