import sys
from PyQt4 import QtGui, QtCore

class Window(QtGui.QMainWindow):
	
	def __init__(self):
		super(Window, self).__init__()
		self.resize(650, 300)
	        self.setWindowTitle("Type 1 diabetes hardware in the loop")
	        self.home()
	
	def home(self):
		btn = QtGui.QPushButton("Aceptar", self)
		btn.clicked.connect(self.close_application)
		btn.resize(btn.minimumSizeHint())
		btn.move(500,250)

                label1 = QtGui.QLabel(" Elija el controlador:", self)
	        label1.setGeometry(300, 10, 180, 20)
		label1.setFont(QtGui.QFont("Calibri",weight=QtGui.QFont.Bold))

                label2 = QtGui.QLabel(" Elija el solver:", self)
                label2.setGeometry(50, 10, 135, 20)
                label2.setFont(QtGui.QFont("Calibri",weight=QtGui.QFont.Bold))

		checkBox1 = QtGui.QCheckBox("Regulador Lineal Cuadratico (LQR)", self)
                checkBox1.resize(checkBox1.sizeHint())
		checkBox1.move(300,50)
		checkBox1.stateChanged.connect(self.enlarge_window)

                checkBox2 = QtGui.QCheckBox("Proporcional-Integral-Derivativo (PID)", self)
                checkBox2.resize(checkBox2.sizeHint())
                checkBox2.move(300,100)
                checkBox2.stateChanged.connect(self.enlarge_window)

                checkBox3 = QtGui.QCheckBox("Euler", self)
                checkBox3.resize(checkBox3.sizeHint())
                checkBox3.move(50,50)
                checkBox3.stateChanged.connect(self.enlarge_window)

                checkBox4 = QtGui.QCheckBox("Runge Kutta Orden 4", self)
                checkBox4.resize(checkBox4.sizeHint())
                checkBox4.move(50,100)
                checkBox4.stateChanged.connect(self.enlarge_window)
	        
	        QtGui.QApplication.setStyle(QtGui.QStyleFactory.create("Cleanlooks"))
		self.setStyleSheet("background-color: aliceblue;")
		btn.setStyleSheet("background-color: lightsteelblue;")
                label1.setStyleSheet("background-color: lightsteelblue; border : 1px inset grey")
		label2.setStyleSheet("background-color: lightsteelblue; border : 1px inset grey")
		self.show()
	
	def close_application(self):
		sys.exit()

	def enlarge_window(self, state):
		if state == QtCore.Qt.Checked:
			self.setGeometry(50, 50, 1000, 600)
 		else:
			self.setGeometry(50, 50, 650, 300)

def run():
	app = QtGui.QApplication(sys.argv)
	GUI = Window()
	sys.exit(app.exec_())

run()
