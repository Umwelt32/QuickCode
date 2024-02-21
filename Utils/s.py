from http.server import BaseHTTPRequestHandler, HTTPServer
import time,subprocess

m_hostName = "localhost"
m_serverPort = 8080
m_scripts_list = ['my_test_python.py']
class MyServer(BaseHTTPRequestHandler):
    def handle_python_script(self,path,argz):
        command = ["python", path]+argz
        output = subprocess.check_output(command)
        output_str = output.decode('utf-8')
        return output_str
    def do_GET(self):
        global m_scripts_list
        try:
            get_req = str(self.path).split('/')
            script_results = self.handle_python_script(m_scripts_list[int(get_req[1])],get_req[2:])
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            self.wfile.write(bytes("<html><head><title>script_runner</title></head>", "utf-8"))
            self.wfile.write(bytes("<p>Request: %s</p>" % self.path, "utf-8"))
            self.wfile.write(bytes("<p>-----------------------</p>", "utf-8"))
            self.wfile.write(bytes("<body>", "utf-8"))
            self.wfile.write(bytes("\n<p>"+script_results+"</p>\n", "utf-8"))
            self.wfile.write(bytes("</body></html>", "utf-8"))
        except Exception as e:
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            self.wfile.write(bytes("<html><head><title>script_runner</title></head>", "utf-8"))
            self.wfile.write(bytes("<p>Request: %s</p>" % self.path, "utf-8"))
            self.wfile.write(bytes("<p>-----------------------</p>", "utf-8"))
            self.wfile.write(bytes("<body><p>Incorrect request!</p><p>"+str(e)+"</p></body></html>", "utf-8"))
if __name__ == "__main__":      
    webServer = HTTPServer((m_hostName, m_serverPort), MyServer)
    print("Server started http://%s:%s" % (m_hostName, m_serverPort))
    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass
    webServer.server_close()
    print("Server stopped.")