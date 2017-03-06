#! /usr/local/bin/python2
import argparse
found = True
try:
    import json
except ImportError:
    print "json not installed"
    found = False
    pass
try:
    import plotly
except ImportError:
    print "plotly not installed"
    found = False
    pass

if __name__ == "__main__":
    if found:
        parser = argparse.ArgumentParser(description='Process function floods.')
        parser.add_argument('--filename', metavar='f', type=str,
                            help='file to load data')
        parser.add_argument('-s', action='store_true')
        args = parser.parse_args()
        print(args)
        if args.filename:
            figure = json.load(file(args.filename))
            kwargs = {}
            if( args.s ):
                kwargs["image"] = "svg"
            filename=args.filename+".html"
            print filename
            plotly.offline.plot(figure, auto_open=False, filename=filename, image_height=800, image_width=800, image_filename=args.filename, **kwargs )
