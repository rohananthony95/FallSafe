import tornado.ioloop
import tornado.web
import tornado.websocket
import os.path

import argparse
import logging
import time
import ast
import math

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

global fallen
fallen = 'OKAY'

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
        global fallen
        if ((self.request.headers['Content-Type'] == 'imagebin')):
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
            fallen = 'OKAY'
            for i, h in enumerate(humans):
                TORSO_INDEX = 1
                LEFT_HIP_INDEX = 8
                RIGHT_HIP_INDEX = 11
                RIGHT_HAND_INDEX = 4
                RIGHT_FOOT_INDEX = 12
                LEFT_FOOT_INDEX = 9
				# and RIGHT_HAND_INDEX in parts and RIGHT_FOOT_INDEX in parts and LEFT_FOOT_INDEX in parts:
            
            parts = h.body_parts
            if TORSO_INDEX in parts and LEFT_HIP_INDEX in parts and RIGHT_HIP_INDEX in parts:
                
                torso = parts[TORSO_INDEX]
                left_hip = parts[LEFT_HIP_INDEX]
                right_hip = parts[RIGHT_HIP_INDEX]
                
                tx, ty = torso.x, torso.y
                lhx, lhy = left_hip.x, left_hip.y
                rhx, rhy = right_hip.x, right_hip.y

                    
                if tx < lhx or tx > rhx:
                    fallen = 'FALL'
            
            
            
                if abs(lhy-ty) < 0.1 or abs(rhy - ty) < 0.1:
                    fallen = 'FALL'
                if  RIGHT_HAND_INDEX in parts and RIGHT_FOOT_INDEX in parts and LEFT_FOOT_INDEX in parts:
                    right_foot = parts[RIGHT_FOOT_INDEX]
                    left_foot = parts[LEFT_FOOT_INDEX]
                    right_hand = parts[RIGHT_HAND_INDEX]
                    righax,righay = right_hand.x,right_hand.y
                    rfx,rfy = right_foot.x,right_foot.y
                    lfx,lfy = left_foot.x,left_foot.y
                    if abs(lfy-lhy) < 0.1 or abs(rhy - ty) < 0.1:
                        fallen = 'FALL'
                    if (lfy-lhy)>(lhy-ty):
                        fallen = 'FALL'
                    print(lfy-lhy,lhy-ty)
                    lowermag = math.sqrt((lfy-lhy)*(lfy-lhy) + (lhx-lfx)*(lhx-lfx))
                    uppermag = math.sqrt((lhy-ty)*(lhy-ty) + (tx-lhx)*(tx-lhx))
                    if lowermag>uppermag:
                        fallen = 'FALL'
            #cv2.putText(image,
            #               f"Fallen: False",
            #              (60, 60),  cv2.FONT_HERSHEY_SIMPLEX, 2,
            #             (0, 255, 0), 5)
                  
            cv2.putText(image,
                    f"Fallen: {fallen}" ,
                    (60, 60),  cv2.FONT_HERSHEY_SIMPLEX, 1,
                    (0, 255, 0), 5)
        
            cv2.imwrite('static/image3.jpg', image)
            for client in clients:
                update_clients(client)
            

class WebcamHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('static/webcam.html')

		
class NurseHandler(tornado.web.RequestHandler):
    def get(self):
        #fallen = 'FALL'
        # self.render('fallinfo.html')
        # print(fallen)
        # if (fallen == True):
            # f = open('fallinfo.html', 'w')
            # f.write('FALL')
            # f.close()
        # elif (fallen == False):
            # f = open('fallinfo.html', 'w')
            # f.write('OKAY')
            # f.close()
        self.write(fallen)
		
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
	(r"/fallinfo", NurseHandler),
], **settings)

if __name__ == "__main__":
    print ('Server Running...')
    print ('Press Ctrl + C to close')
    application.listen(8000)
    tornado.ioloop.IOLoop.instance().start()
