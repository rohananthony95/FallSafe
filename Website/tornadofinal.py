import tornado.ioloop
import tornado.web
import tornado.websocket
import os.path

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
        if (self.request.headers['Content-Type'] == 'imagebin'):
            print 'Received image'
            image = self.request.body
            fh = open('image1.jpg','wb')
            fh.write(bytearray(image))
            fh.close()
            for client in clients:
                update_clients(client)

class WebcamHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('static/webcam.html')

class Websocket1Handler(tornado.websocket.WebSocketHandler):
    def open(self):
        print 'WebSocket1 opened'
        clients.append(self)

    #def on_message(self, message):
        #self.write_message(fh)
        #ws.send(fh)

    def on_close(self):
        print 'WebSocket1 closed'
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
    print 'Server Running...'
    print 'Press Ctrl + C to close'
    application.listen(8000)
    tornado.ioloop.IOLoop.instance().start()
