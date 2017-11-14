
from bottle import route, run, default_app, debug, static_file, redirect, request
from hashlib import md5
from datetime import datetime


class Assignment3(object):
	def __init__(self):
		route('/', 'GET', self.HomePage)
		route('/assignment3/', 'GET', self.Assignment)
		route('/KEO', 'GET', self.KEO)
		route('/time', 'GET', self.Time)
		route('/fibo/<number>', 'GET', self.Fibo)
		route('/fibonacci', 'GET', self.Fibonacci)
		route('/fibonacci', 'POST', self.FiboCal)
		route('/login', 'GET' , self.Login)
		route('/login', 'POST', self.DoLogin)
		route('/grade', 'GET' , self.SelectLesson)
		route('/grade', 'POST', self.PostSelectedLesson)
		route('/grade/<name>/<lesson>/<semester>', 'GET' , self.GradeForm)
		route('/grade/<name>/<lesson>/<semester>', 'POST', self.Calculate)

		route('/static/<filename:path>', 'GET', self.Static)

		self.login = ''
		self.answer = ''
		self.MD5 = lambda text: md5(text.encode('utf-8')).hexdigest()
		self.registration = {}
		self.averages = {key:[0,0] for key in range(1,9)}

	def Template(self, title, content):
		html = """
			<!DOCTYPE html>
			<html>
			  <head>
			      <title>%s</title>
			      <meta charset="utf-8" />
			      <link rel="stylesheet" type="text/css" href="/static/style.css">
			  </head>
			  <body>
			      %s
			  </body>
			</html>"""%(title, content)
		return html

	def Static(self, filename):
		print("keo")
		return static_file(filename, root='.')

	def HomePage(self):
		return self.Template("Welcome to KEO's HomePage",
			'''
			<h1>Welcome to KEO's Website </h1> <br />
			<p><a href="/assignment1/">Click for my assignment 1</a></p>
           	<p><a href="/assignment2/a2_output.html">Click for my assignment 2</a></p>
           	<p><a href="/login">Click for my assignment 3</a></p>''')

	def Assignment(self):
		return self.Template('My lovely assignments',
           """
           <p><a href="/fibonacci">Fibonacci</a></p>
           <p><a href="/grade">Grade Calculator</a></p>
           """)

	def KEO(self):
		return self.Template('KEO ADAMDIR',
			"""<p style="text-align=center;">KEO ADAMDIR</p>""")


	def Fibo(self, number):
		number = int(number)
		l = [0, 1]
		while len(l) < number:
			l.append(l[-1] + l[-2])
		return str(l)

	def Fibonacci(self):
		form = """
			<form action="/fibonacci" method="post">
	            <input type="radio" name="request" value="number" checked="checked" /> Number
	            <input type="radio" name="request" value="list" /> List <br />
	            <input type="text" name="number" placeholder="Enter a Number"> <br />
	            <input value="Get" type="submit" />
	        </form> <br / >
		"""

		if ',' not in self.answer:
			temp = ''
			for part in range(0, len(self.answer), 117):
				try:
					temp += self.answer[part:part + 117] + '- '
				except:
					temp += self.answer[part:]

			self.answer = temp

		answer = """
		<div id="fibans">
			<p>{ans}</p>
		</div>
		""".format(ans=self.answer.strip('[] -').replace('L', ''))

		if self.answer:
			form += answer

		self.answer = ''
		return self.Template('Fibonacci',form)

	def FiboCal(self):
		request_ = request.forms.get('request')
		number = request.forms.get('number')

		try:
			number = int(number)

		except:
			self.answer = 'Please Enter a Number between 0 and 50000'
			redirect('fibonacci')
			return True

		l = [0, 1]
		while len(l) < number+1 and number-1 <= 50000:
			l.append(l[-1] + l[-2])

		if request_ == 'number':
			if 50000 >= number >= 0:
				self.answer = str(l[number])
			else:
				self.answer = 'Please Enter a Number between 0 and 50000'
		else:
			if 50000 >= number >= 0:
				self.answer = str(l[:number+1])
			else:
				self.answer = 'Please Enter a Number between 0 and 50000'
		redirect('fibonacci')


	def Time(self):
		time = '<h2>Current Time: {}</h2>'
		return self.Template('Time', time.format(str(datetime.now())))

	def Login(self):
		f = '''
			<form action="/login" method="post">
	            <input name="username" type="text" placeholder="Username" /> <br />
	            <input name="password" type="password" placeholder="Password" /> <br />

	            <input value="Login" type="submit" />
	            <input type="checkbox" name="register" value="True">Register <br />
	            {status}
	        </form>
		'''.format(status=self.login)

		self.login = ''
		return self.Template('Login Page', f)

	def DoLogin(self):
		username = request.forms.get('username')
		password = request.forms.get('password')
		checked = request.forms.get('register')

		if checked:
			if username not in self.registration:
				self.registration[username] = self.MD5(password)
				redirect('/assignment3/')
				self.login = ''
			else:
				self.login = '<p style="color:red;">Username has taken!</p>'
				redirect('/login')
		else:
			if username in self.registration and self.MD5(password) == self.registration[username]:
				redirect('/assignment3/')
				self.login = ''
			elif username not in self.registration:
				self.login = '<p style="color:red;">This user does not exist.</p>'
				redirect('/login')
			else:
				self.login = '<p style="color:red;">Login failed.</p>'
				redirect('/login')

	def SelectLesson(self):
		form = '''
		<form action="/grade" method="post">
	            <input name="name" type="text" placeholder="Name" /> <br />

	            <select name="lesson">
				  <option value="1">1 Lesson</option>
				  <option value="2">2 Lesson</option>
				  <option value="3">3 Lesson</option>
				  <option value="4">4 Lesson</option>
				  <option value="5">5 Lesson</option>
				  <option value="6">6 Lesson</option>
				  <option value="7">7 Lesson</option>
				  <option value="8">8 Lesson</option>
				  <option value="9">9 Lesson</option>
				  <option value="10">10 Lesson</option>
				</select>

	            <select name="semester">
				  <option value="1">1. Semester</option>
				  <option value="2">2. Semester</option>
				  <option value="3">3. Semester</option>
				  <option value="4">4. Semester</option>
				  <option value="5">5. Semester</option>
				  <option value="6">6. Semester</option>
				  <option value="7">7. Semester</option>
				  <option value="8">8. Semester</option>
				</select> <br />

	            <input value="Submit" type="submit" />
	        </form>
		'''

		return self.Template('Grade Calculate', form)

	def PostSelectedLesson(self):
		name = request.forms.get('name')
		lesson = request.forms.get('lesson')
		semester = request.forms.get('semester')
		if not name.strip() or not name.replace(' ', '').isalpha():
		    redirect('/grade')

		redirect('/grade/{}/{}/{}'.format(name, lesson, semester))

	def GradeForm(self, name, lesson, semester):
		credits = '''
		<select name="credit{n}">
			<option value="1"> 1 </option>
			<option value="2"> 2 </option>
			<option value="3"> 3 </option>
			<option value="4"> 4 </option>
			<option value="5"> 5 </option>
		</select>'''

		grades = '''
		<select name="grade{n}">
			<option value=  "4"> AA </option>
			<option value="3.5"> BA </option>
			<option value=  "3"> BB </option>
			<option value="2.5"> CB </option>
			<option value=  "2"> CC </option>
			<option value="1.5"> DC </option>
			<option value=  "1"> DD </option>
			<option value=  "0"> FF </option>

		</select> <br />\n'''

		rows = ''
		for _ in range(1, int(lesson)+1):
			row = '{n}. Lesson' + credits + grades
			row = row.format(n=_)
			rows += row

		form = '''
		<h2>Hi {name}</h2>
		<form action="/grade/{name}/{lesson}/{semester}" method="post">
				{rows}

				<input type="checkbox" name="show" value="me">My Average
				<input type="checkbox" name="show" value="all">General Average
	            <input value="Submit" type="submit" />
	        </form>
		'''.format(name=name, lesson=lesson, semester=semester,rows=rows)

		return self.Template('Enter Grades', form)

	def Calculate(self, name, lesson, semester):
		lesson = int(lesson)
		semester = int(semester)
		s = 0
		c = 0

		for _ in range(1, lesson+1):
			credit = request.forms.get('credit{}'.format(_))
			grade = request.forms.get('grade{}'.format(_))
			shows = request.forms.getall('show')

			s += float(credit) * float(grade)
			c += float(credit)

		result = s / c
		self.averages[semester][1] += 1
		self.averages[semester][0] = (self.averages[semester][0]*(self.averages[semester][1]-1) + result) / self.averages[semester][1]

		content = ''
		if 'me' in shows:
			content += '<h1>Your average is {:1.3f} </h1>'.format(result)
		if 'all' in shows:
			content += '<h1>General average is {:1.3f} </h1>'.format(self.averages[semester][0])

		if len(shows) == 0:
			redirect('http://hasgule15itu2015.pythonanywhere.com/assignment3/')

		if result > 2.5:
			title = 'Congratulations!'
		else:
			title = 'Not bad, not bad!'
		return self.Template(title, content)

	def Run(self):
		run()

assignment3 = Assignment3()

debug(True)
application = default_app()

if __name__ == '__main__':
	assignment3.Run()