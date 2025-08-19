from cairosvg import svg2png
from os import listdir
from os.path import isfile, join

onlyfiles = [f for f in listdir("") if isfile(join("", f))]
print(onlyfiles)

svg_code = open("tree.dot.svg", 'rt').read()
svg2png(bytestring=svg_code,write_to='output.png')