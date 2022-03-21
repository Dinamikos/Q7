#class creation
class RegularExpressionProcessor():
    '''
    manage strings to find, replace and delete matches 
    in regular expressions
    '''

    def initialize(self):
        return 1
    
    def find(self, regex, string):
        '''
        this method return an array with all the matches
        of the regular expression in the string passed 
        as a parameter.
        '''

    def replace(self, regex, string, value):
        '''
        this method recieves a regular expression, find 
        all the matches in the given string and replace 
        those matches with the value string
        '''
        
    def delete(self, regex, string):
        '''
        this method return a version of the passed string
        with all the matches of the regular expression 
        supressed.
        '''

    def process(self, data):
        '''
        this method receive a iterable set of strings and
        return a version processed with all the regular
        expressions applied in order to eahc of them.
        '''

#testing
engine = RegularExpressionProcessor()
