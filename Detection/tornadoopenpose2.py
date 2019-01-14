import tornado.ioloop
import tornado.web
import tornado.websocket
import os.path

import argparse
import logging
import time
import ast

import common
import cv2
import numpy as np
from estimator import TfPoseEstimator
from networks import get_graph_path, model_wh

logger = logging.getLogger('TfPoseEstimator')
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('[%(asctime)s] [%(name)s] [%(levelname)s] %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

global finished_post
finished_post = 1

clients = []

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('index.html')

class InfoHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('info.html')

settings = dict(
        static_path=os.path.join(os.path.dirname(__file__), "static"),
)

class ImageHandler(tornado.web.RequestHandler):
    def post(self):
        global finished_post
        if ((self.request.headers['Content-Type'] == 'imagebin') and (finished_post == 1)):
            print(finished_post)
            finished_post = 0
            print ('Received image')
            image = self.request.body
            fh = open('static/image1.jpg','wb')
            fh.write(image)
            fh.close()
            #fh = open('static/image1.jpg','ab')
            #fh.write(bytes([0xD9]))
            #fh.close()
            print('0')
            #image = cv2.imread('static/image1.jpg')
            print ('1')
            print('2')
    
            parser = argparse.ArgumentParser(description='tf-pose-estimation run')
            parser.add_argument('--resolution', type=str, default='432x368', help='network input resolution. default=432x368')
            parser.add_argument('--model', type=str, default='mobilenet_thin', help='cmu / mobilenet_thin')
            parser.add_argument('--scales', type=str, default='[None]', help='for multiple scales, eg. [1.0, (1.1, 0.05)]')
            args = parser.parse_args()
            scales = ast.literal_eval(args.scales)

            w, h = model_wh(args.resolution)
            e = TfPoseEstimator(get_graph_path(args.model), target_size=(w, h))

            # estimate human poses from a single image !
            image = common.read_imgfile('static/image1.jpg', None, None)
            # image = cv2.fastNlMeansDenoisingColored(image, None, 10, 10, 7, 21)
            t = time.time()
            humans = e.inference(image, scales=scales)
            elapsed = time.time() - t

            logger.info('inference image: image3.jpg in %.4f seconds.' % (elapsed))

            image = TfPoseEstimator.draw_humans(image, humans, imgcopy=False)
    
            #cv2.putText(image,
             #               f"Fallen: False",
              #              (60, 60),  cv2.FONT_HERSHEY_SIMPLEX, 2,
               #             (0, 255, 0), 5)
                    
            cv2.imwrite('static/image3.jpg', image)
 
            for client in clients:
                update_clients(client)
             
            print(finished_post)			 
            finished_post = 1

class WebcamHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('static/webcam.html')

class Websocket1Handler(tornado.websocket.WebSocketHandler):
    def open(self):
        print ('WebSocket1 opened')
        clients.append(self)

    #def on_message(self, message):
        #self.write_message(fh)
        #ws.send(fh)

    def on_close(self):
        print ('WebSocket1 closed')
        clients.remove(self)

def update_clients(self):
    self.write_message(".")

application = tornado.web.Application([
    (r"/", MainHandler),
    (r"/webcam", WebcamHandler),
    (r"/image", ImageHandler),
    (r"/info", InfoHandler),
    (r"/socket", Websocket1Handler),
], **settings)

if __name__ == "__main__":
    print ('Server Running...')
    print ('Press Ctrl + C to close')
    application.listen(8000)
    tornado.ioloop.IOLoop.instance().start()
