import subprocess

def check(expr, expect):
    out = subprocess.run(['./calc', expr], stdout=subprocess.PIPE)
    result = out.stdout.decode("utf-8")
    
    if result != expect+'\n':
        print('error in test: {0} != {1}, got {2}\n'.format(expr, expect, result))
        
err_str = 'BAD INPUT'

check('2+2', '4')
check('100', '100')
check('-78901', '-78901')

check('', err_str)
check('2+a', err_str)
check('hello world', err_str)
check(' 25 + 54 2', err_str)
check('+ 3', err_str)

check(' - 3', '-3')
check(' 30 +  - 2    ', '28')
check(' - - -    - - - 69000', '69000')


print("Test is finished!")
