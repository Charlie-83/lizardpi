#!/usr/bin/python
import http.server
import socketserver
import matplotlib.pyplot as plt
from urllib.parse import urlparse

class RequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        parsed_path = urlparse(self.path)

        if parsed_path.path == "/":
            with open("/home/charlie/temp.log", "r") as f:
                temperatures = [int(line.strip()) for line in f if line.strip()]
            with open("/home/charlie/humidity.log", "r") as f:
                humidities = [int(line.strip()) for line in f if line.strip()]

            plt.figure()
            plt.plot(temperatures, marker="o")
            plt.title("Temperatures")
            plt.xlabel("Index")
            plt.ylabel("Value")
            plt.savefig("/home/charlie/temperatures.png")
            plt.close()

            plt.figure()
            plt.plot(humidities, marker="o")
            plt.title("Humidities")
            plt.xlabel("Index")
            plt.ylabel("Value")
            plt.savefig("/home/charlie/humidities.png")
            plt.close()

            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            html = """
            <!doctype html>
            <html lang="en">
              <head>
                <meta charset="utf-8">
                <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
                <title>Tank Stats</title>
              </head>
              <body>
                <img src="/temperatures" alt="Plot of temperatures">
                <img src="/humidities" alt="Plot of humidities">
              </body>
            </html>
            """
            self.wfile.write(html.encode())
        elif parsed_path.path == "/temperatures":
            self.send_response(200)
            self.send_header("Content-type", "image/png")
            self.end_headers()
            with open("/home/charlie/temperatures.png", "rb") as f:
                self.wfile.write(f.read())
        elif parsed_path.path == "/humidities":
            self.send_response(200)
            self.send_header("Content-type", "image/png")
            self.end_headers()
            with open("/home/charlie/humidities.png", "rb") as f:
                self.wfile.write(f.read())
        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b"404 Not Found")

socketserver.TCPServer(("", 1030), RequestHandler).serve_forever()
