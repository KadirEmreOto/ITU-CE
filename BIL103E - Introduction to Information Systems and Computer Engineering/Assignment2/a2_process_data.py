#######################################################
### Please ignore the following lines of code.
### It loads the contents of a CSV file for you.
### The file's name should be a2_input.csv.
### You do NOT need to know how it works.
#######################################################

import csv, random

contents = []
with open("a2_input.csv") as input_file:
    for row in csv.reader(input_file):
        contents = contents + [row]

#######################################################
### Do your data processing below.
### The below code gives some examples
### of how to access the data.
### Print your results using the print function.
#######################################################

start = '''
<!DOCTYPE html>
<html>
<head>
	<title>Cars</title>
	<meta charset="UTF-8" />
	<style>

		body {
			font-family: 'Lato', monospace;
			background-color: #282525; /*#f4f1ea;*/
		}

		p {
			font-size: 100%;
			color: green; 
			text-align: center;
		}

		table#Table {
			margin-top: 41px;
			margin-left: auto;
		    margin-right: auto;
			
			font-size: 120%;
			color: green; 
			text-align: center;
				
			border-spacing: 3px;
			border-collapse: separate;
		}

		table#Table td, th {
			padding: 5px;
			background-color: #3A3A3A;
		}

		hr {
			border-style: inset;
		}
	</style>
</head>
<body>
<h1 style="color:green; text-align:center;">Cars</h1>
'''
print(start)




### Table #########################################
th_check = False
print('\t<table id="Table">')

for row in contents:
	print('\t\t<tr>')
	for item in row:
		if not th_check:
			print('\t\t\t<th>{}</th>'.format(item))
		else:	
			print('\t\t\t<td>{}</td>'.format(item))
	th_check = True
	print('\t\t</tr>')

print('\t</table>')
###################################################




## Statistics ########################################################################################
number_of_car = len(contents) -2 
random_car1 = contents[random.randint(2, 100)][0]
random_car2 = contents[random.randint(100, 200)][0]
print('\t<p>There are {} cars such as {}, {}</p>'.format(number_of_car, random_car1, random_car2))

heaviest = (0, 0) 
r_index = 2
while r_index < len(contents):
	weight = float(contents[r_index][5])
	if weight > heaviest[1]:
		heaviest = (r_index, weight)

	r_index += 1
print('\t<p>Heaviest Car is {} with {} kg</p>'.format(contents[heaviest[0]][0], heaviest[1]))

number_of_us = 0
SumofAcceleration = 0
for row in contents:
	try:
		acceleration = float(row[6])
		SumofAcceleration += acceleration
	except:
		pass

	origin = row[8]
	if origin == 'US':
		number_of_us += 1

print('\t<p>Average of acceleration of all cars is {} </p>'.format(SumofAcceleration / number_of_car))
print('\t<p>The number of cars manufactured in the US is {}</p>\n'.format(number_of_us))
######################################################################################################



print('<hr / >')


"""
print(contents)
print(contents[0])
print(contents[0][0])
print(type(contents))
print(type(contents[0]))
print(type(contents[0][0]))
print(contents[6][0], contents[6][1])
print(contents[7][0], contents[7][1])
print(type(contents[2][2]))
print(type(contents[5][3]))

WeightSum = 0
for row in contents[2:]:
	WeightSum += float(row[5])

WeightAverage = WeightSum / len(contents - 2)


print(WeightSum)
print(WeightAverage)

# contents["chickenchicken"] -> TypeError: list indices must be integers, not str

try:
	contents["chickenchicken"]
except Exception as e:
	print e


titles = contents[0][0] + ' ' + contents[0][1] + ' ' + contents[0][2] + ' ' + contents[0][3] + ' ' + contents[0][4]
print(titles)


# 3 + '3' -> TypeError: unsupported operand type(s) for +: 'int' and 'str'
print(3 * 'KEO')

print(type(3*'KEO'))
print(type(WeightSum))
help(WeightSum)
"""


print('</body></html>')
