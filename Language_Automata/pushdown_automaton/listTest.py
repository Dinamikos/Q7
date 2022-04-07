text = 'aa'
#get the last characters
print(text[1:])
text = text[1:]
print(text[1:])

'''
# if the word finish but queue is not empty
  False
# if the queue is empty but the word is not finish
  False
# if the word is finish and the queue is empty and the current state is in the final states
  True

# Read char
# pop in queue OUTO
# put in queue INTO
# current state next state
# call the function again str[1:]
# pop in queue INTO
# put in queue OUTO
# current state previous state
'''