import random

commands = ['left', 'right', 'down', 'clockwise', 'counterclockwise',
 'drop', 'levelup', 'leveldown', 'norandom', 'sequence', 'restart',
 'I', 'J', 'L', 'S', 'Z', 'O', 'T']


with open('./tests/thicc.in', 'w') as file:
    for i in range(1000):
        command = commands[random.randrange(0, len(commands))]
        if command == 'norandom':
            command += ' sequence.txt'
        elif command == 'sequence':
            command += ' tests/score.in'
        else:
            command = command[:random.randrange(0, len(command))]
            multiplier = random.randrange(20)
            command = f'{multiplier}{command}'
        file.write(command + '\n')

    file.write('quit' + '\n')
