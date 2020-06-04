import socket;
import binascii
import tkinter as tk
from PIL import ImageTk
from PIL import Image
import pdb
import threading

hostname = "martmattone.wifi.local.cmu.edu";
port = 42069;

hostname = "localhost"
port = 8080

winwidth = 2400;
winheight = 1500;

class SimpleApp(object):
    def __init__(self, master, filename, **kwargs):
        self.master = master
        self.filename = filename
        self.canvas = tk.Canvas(master, width=winwidth, height=winheight)
        self.canvas.pack()
        self.leg_data = [ 0, 0, 0, 0, 0, 0 ];
        self.cmd_data = [ 0, 0, 0, 0, 0, 0 ]

        self.update = self.draw().__next__
        master.after(100, self.update)

    def draw(self):
        fwheg_img = Image.open(self.filename)
        flip_fwheg = Image.open("flipped_fwheg.png")
        while True:
            self.canvas.create_rectangle(winwidth/16, winheight/8, winwidth/2 - winwidth/16, winheight/4, fill="#05f");
            self.canvas.create_rectangle(winwidth/2 + winwidth/16, winheight/8, winwidth - winwidth/16, winheight/4, fill="#05f")
            self.canvas.create_rectangle(winwidth/16, winheight/2 + winheight/8, winwidth/2 - winwidth/16, winheight/2 + winheight/4, fill="#f50")
            self.canvas.create_rectangle(winwidth/2 + winwidth/16, winheight/2 + winheight/8, winwidth - winwidth/16, winheight/2 + winheight/4, fill="#f50")
            self.canvas.create_text(winwidth/2, winheight/16, font=("Arial", 40), text="Current Leg Positions")
            self.canvas.create_text(winwidth/2, winheight/2 + winheight/16, font=("Arial", 40), text="Commanded Leg Positions")

            # front left current
            flfwheg = ImageTk.PhotoImage(fwheg_img.rotate(self.leg_data[0]));
            self.canvas.create_image(winwidth/8, winheight/4 - winheight/16, image=flfwheg)
            # mid left current
            mlfwheg = ImageTk.PhotoImage(fwheg_img.rotate(self.leg_data[1]))
            self.canvas.create_image(winwidth/4, winheight/4 - winheight/16, image=mlfwheg)
            # back left current
            blfwheg = ImageTk.PhotoImage(fwheg_img.rotate(self.leg_data[2]))
            self.canvas.create_image(winwidth/4 + winwidth/8, winheight/4 - winheight/16, image=blfwheg)
            # front right current
            frfwheg = ImageTk.PhotoImage(flip_fwheg.rotate(self.leg_data[3]))
            self.canvas.create_image(winwidth/2 + winwidth/8, winheight/4 - winheight/16, image=frfwheg)
            # mid right current
            mrfwheg = ImageTk.PhotoImage(flip_fwheg.rotate(self.leg_data[4]))
            self.canvas.create_image(winwidth/2 + winwidth/4, winheight/4 - winheight/16, image=mrfwheg)
            # back rigth current
            brfwheg = ImageTk.PhotoImage(flip_fwheg.rotate(self.leg_data[5]))
            self.canvas.create_image(winwidth/2 + winwidth/4 + winwidth/8, winheight/4 - winheight/16, image=brfwheg)

            # front left comm
            flcomm = ImageTk.PhotoImage(fwheg_img.rotate(self.cmd_data[0]))
            self.canvas.create_image(winwidth/8, winheight/2 + winheight/4 - winheight/16, image=flcomm)
            # mid left comm
            mlcomm = ImageTk.PhotoImage(fwheg_img.rotate(self.cmd_data[1]))
            self.canvas.create_image(winwidth/4, winheight/2 + winheight/4 - winheight/16, image=mlcomm)
            # back left comm
            blcomm = ImageTk.PhotoImage(fwheg_img.rotate(self.cmd_data[2]))
            self.canvas.create_image(winwidth/4 + winwidth/8, winheight/2 + winheight/4 - winheight/16, image=blcomm)
            # front right comm
            frcomm = ImageTk.PhotoImage(flip_fwheg.rotate(self.cmd_data[3]))
            self.canvas.create_image(winwidth/2 + winwidth/8, winheight/2 + winheight/4 - winheight/16, image=frcomm)
            # mid right comm
            mrcomm = ImageTk.PhotoImage(flip_fwheg.rotate(self.cmd_data[4]))
            self.canvas.create_image(winwidth/2 + winwidth/4, winheight/2 + winheight/4 - winheight/16, image=mrcomm)
            # back right comm
            brcomm = ImageTk.PhotoImage(flip_fwheg.rotate(self.cmd_data[5]))
            self.canvas.create_image(winwidth/2 + winwidth/4 + winwidth/8, winheight/2 + winheight/4 - winheight/16, image=brcomm)

            spim = Image.open("scienceparrot.png");
            scipar = ImageTk.PhotoImage(spim)
            self.canvas.create_image(winwidth/2, winheight/2 - winheight/16, image=scipar);

            self.master.dooneevent();

            self.master.after_idle(self.update)
            yield

root = tk.Tk()
app = SimpleApp(root, 'sampleimg.png')

def data_connection():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    s.connect((hostname, port));

    while True:
        data = s.recv(1024);
        if data:
            print(binascii.hexlify(data))
            # pdb.set_trace()
            data = [c for c in data]
            headerbyte = data[0];
            if (headerbyte == 0x69):
                leg_data = [];
                i = 1;
                for j in range(6):
                    upperbyte = int(data[i]);
                    i += 1;
                    lowerbyte = int(data[i]);
                    i += 1

                    legval = (upperbyte << 8) + lowerbyte;
                    leg_data.append(legval);
                
                # leg data is in ticks, convert to angle
                for i in range(len(leg_data)):
                    leg_data[i] = int(leg_data[i] * 360 / 4096)
                    leg_data[i] += 180;

                app.leg_data = leg_data;

            elif (headerbyte == 0x42):
                cmd_data = [];
                i = 1;
                for j in range(7):
                    upperbyte = int(data[i]);
                    i += 1;
                    lowerbyte = int(data[i]);
                    i += 1
                
                    cmdval = (upperbyte << 8) + lowerbyte;
                    cmd_data.append(cmdval);

                for i in range(len(cmd_data)):
                    cmd_data[i] = int(cmd_data[i] * 360 / 4096);
                    cmd_data[i] += 180;
                
                app.cmd_data = cmd_data
            else:
                pass


threading.Thread(target=data_connection).start()

root.mainloop()