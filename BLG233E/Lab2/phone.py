import os
from bisect import bisect

class Record(object):
	name = ''
	cell = ''

class PhoneBook(object):
	def __init__(self):
		self.filename = 'tel_defteri.txt'
		self.records = []
		self.kayitsayisi = 0

	def read(self):
		self.kayitsayisi = 0

		if self.filename not in os.listdir(os.getcwd()):
			with open(self.filename, 'w') as file: return

		with open(self.filename , 'r') as file:
			source = file.read().strip()

		for line in source.split('\n'):
			line = line.strip()
			if not line: continue

			new = Record()
			new.name, new.cell = line.strip().split()

			self.add(new)

	def save(self):
		with open(self.filename, 'w+') as file:
			for name, rec in self.records:
				file.write('{}\t{}\n'.format(rec.name, rec.cell))

	def add(self, record):
		item = (record.name.lower(), record)
		self.records.insert(bisect(self.records, item), item)
		self.kayitsayisi += 1

	def delete(self, index):
		del self.records[index-1]
		self.kayitsayisi -= 1

	def update(self, index, record):
		self.delete(index)
		self.add(record)

	def search(self, name):
		if name == '*':
			self.list()
			return

		count = 0
		index = 0

		for n, rec in self.records:
			index += 1
			if rec.name.startswith(name):
				print '{}. {}\t{}'.format(index, rec.name, rec.cell)
				count += 1

		if not count:
			print 'Record can not be found'


	def list(self):
		for i in xrange(self.kayitsayisi):
			print '{}. {}\t{}'.format(i+1, self.records[i][1].name, self.records[i][1].cell)

		if not self.kayitsayisi:
			print 'The Phone List is empty!'


class Program(object):
	def __init__(self):
		self.book = PhoneBook()

	def run(self):
		self.book.read()

		while True:
			self.display()
			self.userinput()

	def display(self):
		os.system('clear')
		print """
	Phone Book Application
Choose an operation
A: Search Record
E: Add Record
G: Update Record
S: Delete Record
L: List Records
C: Exit

Enter an option {A, E, G, S, C, L} :""",

	def userinput(self):
		opt = raw_input().lower()

		if opt == 'a':
			name = raw_input("Please enter the name of the person you want to update (press '*' for listing all):")
			self.book.search(name)
			raw_input()

		elif opt == 'e':
			print "Please enter the information of the person you want to save "

			rec = Record()
			rec.name = raw_input('Name: ')
			rec.cell = raw_input('Cell: ')

			self.book.add(rec)

		elif opt == 'g':
			name = raw_input("Please enter the name of the person you want to update (press '*' for listing all): ")
			self.book.search(name)

			index = int(raw_input('\nIndex: '))
			rec = Record()
			rec.name = raw_input('New Name: ')
			rec.cell = raw_input('New Cell: ')

			self.book.update(index, rec)

		elif opt == 's':
			name = raw_input("Please enter the name of the person you want to update (press '*' for listing all): ")
			self.book.search(name)

			index = raw_input('Index: ')
			self.book.delete(int(index))

		elif opt == 'l':
			self.book.list()
			raw_input()

		elif opt == 'c':
			self.book.save()
			exit()

if __name__ == '__main__':
	app = Program()
	app.run()
