import sys

size = int(sys.argv[-1])
array = []
header = True

with open('population_by_zip_2010.csv') as file:
	for line in file:
		if header:
			header = False
			continue

		if len(array) >= size:
			break

		a = line.split(',')
		a[-1] = a[-1].strip()
		a[0] = int(a[0]) if a[0] else '' 
		a[1] = int(a[1]) if a[1] else '' 
		a[2] = int(a[2]) if a[2] else '' 
		a[4] = int(a[4]) if a[4] else '' 
		a.append(len(array))
		array.append(a)

array.sort(key=lambda item: (item[0], item[5], item[6]))

with open('answer.csv', 'w') as file:
	file.write('population,minimum_age,maximum_age,gender,zipcode,geo_id\r\n')

	for item in array:
		file.write(','.join(map(str, item[:-1])) + '\r\n')
