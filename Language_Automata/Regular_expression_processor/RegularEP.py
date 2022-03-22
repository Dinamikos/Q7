import re

#class creation
class RegularExpressionProcessor:
    '''
    manage strings to find, replace and delete matches 
    in regular expressions
    '''

    def __init__(self, string):
        #This strings needs to be declared when the object is initilized
        #the idea is that each time that you access to process, self.string
        #is used
        self.string = string
        #NOTE code with self.string, or use the example below
    
    def find(self, regex:str, string:str) -> list:
        '''
        this method return an array with all the matches
        of the regular expression in the string passed 
        as a parameter.
        '''
        re.findall(regex, string)
        

        return #TODO Matches index JM guess

    def replace(self, regex:str, string:str, value:str) -> str:
        '''
        this method recieves a regular expression, find 
        all the matches in the given string and replace 
        those matches with the value string
        '''
        
        return "new String with regex replaced by value"

    def delete(self, regex:str, string:str) -> str:
        '''
        this method return a version of the passed string
        with all the matches of the regular expression 
        supressed.
        '''

        return "new String with regex deleted"
        
    def _regexValidation(self, data):
        '''
        In order to regex, it will check if all conditions
        are applied in the data dictionary
        '''

        if type(data) != list:
            print("It needs to be a list as: [['delete','this','in this']]")
            return False

        for rule in data:
            length = len(rule)#4 for replace
            opt = rule[0]#['replace','regex','str','value']

            if opt == "delete" and length == 2:
                return True
            elif opt == "replace" and length == 3:
                return True
            elif opt == "find" and length == 2:
                return True

        return False

    def process(self, data, apply=False):
        '''
        this method receive a iterable set of strings and
        return a version processed with all the regular
        expressions applied in order to eahc of them.
        '''
        #check all the rules
        if not self._regexValidation(data):
            print("your data list is not correct, check it")
            return

        aux = self.string

        for regex in data:
            opt = regex[0] #option replace, delete, find
            regularEX = regex[1] # regular expression
            if opt == "replace":
                aux = self.replace(regularEX, aux, regex[2])
            elif opt == "find":
                return self.find(regularEX, aux)
            elif opt == "delete":
                aux = self.delete(regularEX, aux)
        
        if apply:
            self.string = aux
        else:
            return aux


#testing
engine = RegularExpressionProcessor("Hola como estas juanaaa@gmail.com osirisaaa@gmail.com victor@gmail.com isaa@gmail.com hola commo estás")
engine = engine.process([['delete','/\w+a{3}@gmail\.com/']])
##engine.find('/\w+a{3}@gmail\.com/', engine.string)
#Osiris hola