import robot
import argparse



if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Lance Rosebot')
    parser.add_argument('-l', '--log', help='active le log', action="store_true")
    parser.add_argument('-c', '--vert', nargs=1,  help='robot vert ?', action="store_true")
    parser.add_argument('methode', nargs='?', default='lancer')
    
    args = parser.parse_args()
    
    rosebot = robot.Robot(log=args.log, jaune=not args.vert)

    getattr(rosebot, args.methode)
    