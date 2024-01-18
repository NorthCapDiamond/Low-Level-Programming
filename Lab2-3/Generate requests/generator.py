import pandas as pd 

user = pd.read_csv("/Users/lovestory/Desktop/itmo/llp/low_level_programming_lab1/Lab3/Generate requests/export-2.csv", nrows=1000)
user["n.display_name"].fillna("Anonim", inplace = True)
message = pd.read_csv("/Users/lovestory/Desktop/itmo/llp/low_level_programming_lab1/Lab3/Generate requests/export.csv", nrows=1000)

namedb = "data.db"
answer = ""
link = ""

selecter = ""

f = open("codes.graphql", "w")


user_id = 0

size_of_user = user.shape[0]
print(size_of_user)
size_of_message = message.shape[0]
links = ""


collection = {}

def generate_insert(line):
	global namedb
	global answer
	var = "%s{\ninsert{\nnode(values : [0, 0.0, %s, \"%s\"])\n}}" % (namedb, line[0], line[1][1:-1])
	answer = answer + "\n" + var +"\n"

	return line[0][1:-1]

def link_nodes(id1, id2):
	global link
	global namedb
	var = "%s{link(%s, %s)}" % (namedb, str(id1), str(id2))
	link = link + "\n" + var + "\n"

def linkers(id_l):
	global namedb
	global links
	var = "%s{all_links(%s)}" % (namedb, str(id_l))
	links = links + "\n" + var + "\n"


def generate_select(id):
	global namedb
	global selecter
	var = "%s{ select{\nindex (id : %s){\nfilter : eq(some_attr, 1)\n}}}" % (namedb, str(id))
	selecter = selecter + "\n" + var +"\n"


for i in range(size_of_user):
	line = user.loc[i]
	name = generate_insert(line)
	user_id+=1

	collection[str(name)] = user_id


for i in range(size_of_message):
	line = message.loc[i]
	name = line[0].split()[0][3:-1]
	print(name)
	if name in collection:
		generate_insert(line).split()[0][2:-1]
		user_id+=1
		a = collection[name]
		if (a!=user_id):
			link_nodes(a, user_id)
			linkers(user_id)
			generate_select(user_id)


f.write(answer)
f.close()

f2 = open("commands.graphql", "w")
f2.write(link)
f2.close()

f3 = open("select.graphql", "w")
f3.write(selecter)
f3.close()

f4 = open("links.graphql", "w")
f4.write(links)
f4.close()
			
