
from rosebotnav import Vec

Pieds = [(200,90), (1750,90), (1850,90), (1770,1100), (100,850), (200,850), (1355,870), (1400,1300)]
PiedsJaune = [ (a/1000.,b/1000.) for a,b in Pieds ]

def convCoul(pos):
    x,y = pos
    y = 3-y
    
    if isinstance(pos, tuple):
        return (x,y)
    elif isinstance(pos, list):
        return [x,y]
    else:
        return type(pos)(x,y)
    
PiedsVerts = [ convCoul(pos) for pos in PiedsJaune ]
 
Gobelets = (.8,.91), (.800,2.090), (1.750,.250), (1.650,1.500), (1.750,2.750)

CoinEscalier = Vec(.580,.967)

if __name__ == '__main__':

    print convCoul((1,1))
    print convCoul([1,1])
    print convCoul(Vec(1,1))
    