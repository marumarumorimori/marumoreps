#!bin/python

class PrintTest:
	_list = ["Hello","World"]

	def __init__(self,one_phrase):
		words = one_phrase
	#__init__ End

	def Print(self):
		print self._list[0],self._list[1] 
	#Print End

#class PrintTest

do = PrintTest("Yeah!")
do.Print()
