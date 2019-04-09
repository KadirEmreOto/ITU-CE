from collections import Counter

with open('stopwords.txt', 'r') as file:
	stopwords = set(file.read().split())

with open('input.txt', 'r') as file:
	frequency = Counter(filter(lambda x: x in stopwords, file.read().split()))

total = float(sum(frequency.values()))
for word in sorted(frequency.keys(), key=lambda x: -frequency[x]):
	print word, frequency[word], '%%%s'%(frequency[word] / total * 100)
