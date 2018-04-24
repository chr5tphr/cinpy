import minext

# get version, function returns a python string
version = minext.version()
print(version)

# call some program using system
minext.system('/bin/date')

