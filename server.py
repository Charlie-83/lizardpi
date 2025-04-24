#!/usr/bin/python
import http.server
import socketserver
import matplotlib.pyplot as plt
import csv
from datetime import datetime, timedelta
from urllib.parse import urlparse

class RequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        parsed_path = urlparse(self.path)

        if parsed_path.path == "/":
            times = []
            temperatures = []
            humidities = []
            with open("/home/charlie/lizard.log", "r") as f:
                reader = csv.reader(f)
                for row in reader:
                    times.append(int(row[0]))
                    temperatures.append(int(row[1]))
                    humidities.append(int(row[2]))

            datetimes = [datetime.fromtimestamp(ts) for ts in times]
            now = datetime.now()
            yesterday = now - timedelta(days=1)
            temperatures = [t for i, t in enumerate(temperatures) if yesterday <= datetimes[i] <= now]
            humidities = [t for i, t in enumerate(humidities) if yesterday <= datetimes[i] <= now]
            datetimes = [d for d in datetimes if yesterday <= d <= now]

            plt.figure()
            plt.plot(datetimes, temperatures, marker="o")
            plt.title("Temperatures")
            plt.xlabel("Index")
            plt.ylabel("Value")
            plt.savefig("/home/charlie/temperatures.png")
            plt.close()

            plt.figure()
            plt.plot(datetimes, humidities, marker="o")
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
