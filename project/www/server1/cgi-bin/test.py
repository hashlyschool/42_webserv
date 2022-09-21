from pathlib import Path

path = '.'

print("Content-Type: text/plain")
print()
print("I am cgi python script.\nls:")
for path in Path(path).iterdir():
	print(path)
print()
