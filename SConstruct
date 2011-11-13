
# Core Library
env = Environment()
env.Library('sib', Glob('src/main/**/*.cpp'))

# Test
print Glob(includes='*.cpp', None, dir='src/test') #'src/test/*.cpp')
env.Library('sibtest', Glob('src/test/**/*.cpp'))

