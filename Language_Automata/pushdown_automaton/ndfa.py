# Importing libraries
import os
import itertools

class NonDeterminsiticAutomaton:
    '''
    Class to instantiate the properties in the NonDeterminsiticAutomaton object
    '''
    def initialize(self, q, sigma, delta, q0, empty, f):
        '''
        this method receives the automaton elements and set its own properties

        
        Parameters:
        - Self: Stands for the crrent object Automata 
        - q: List of strings, this is the automaton states like {q0,q1,q2..qn}
        - sigma: The alphabet, this is a list of strings
        - delta: Dictionary that stands transitions from one state to another. 
        - q0: Strings of the unique initial state
        - empty: String representating the empty transition in the automaton. 
        - f: List of strings of all the final states
        
        OUTPUT: 
        No output, it only stores the elements within the object Automaton.
        '''
        # Store the values of q
        self.q= q
        # Store the values of sigma
        self.sigma= sigma
        # Store the values of delta
        self.delta =self.set_delta(delta)
        # Store the values of q0
        self.q0 =q0
        
        #Store the empty value
        self.empty= empty
        # Store the values of f
        self.f= f
        #current state of the word
        self.current = self.q0
        self.path=[]
        self.lastpath=[]
        
        ##Element useful to processing_word function
        self.wordVal= False

        #Validation of my automaton
        self.is_valid(q, sigma, self.delta, q0, f)
    
    def set_delta(self, delta):
        new_D={}
        if type(delta) == str:
            delta = delta.split(sep=',')

        ##----# Remember: 'q0-0':q1, so I need to do a separation 
        #print(type(delta))
        #d= []
        for rule in delta:
            rule= rule.split(sep='-')
            #print(rule)
            a= rule[0]+ '-' + rule[1]
            ##b= rule[2]
            try:
                
                ##l= len(new_D[a])
                new_D[a].append(rule[2])
                
            except:
                new_D[a]= [rule[2]]
            ##new_D[a]= b
        #print(new_D)
        return new_D
    
    def load_from_file(self, name_file):
        '''
        this method receives a txt file with the automaton defninition and loads the properties.
        
        PARAMETERS: 
        - name_file: A string type standing for the file's with txt termination. e.g 'filename.txt'
        
        OUTPUT:
        The automata, but not in a structured way.
        '''
        ##self.current = self.q0
        #Open the file and separate in lines
        with open(name_file) as f:
            contents = f.read().splitlines()
            ##print(contents)
        
        
        new_contents= []
        ##Enter to the line in the txt where each element of the automaton should be and apply the correspondant changes of replace function due the structure of the file
        for arg in contents:
            new_contents.append(arg.replace('{','').replace('}','').replace(' ','').split(sep=','))

        ##new_q= new_contents[0] ( I just want to have q and sigma)
        self.q= new_contents[0]

        ##new_sigma= new_contents[1]
        self.sigma= new_contents[1]

        
        # Changes to delta because is a dictionary of values
        new_d= new_contents[2]
        new_D={}


        # Store delta values
        self.delta= self.set_delta(new_d)
        
        #Sote empty value
        
        self.empty = self.get_empty(self.delta)

        # Store q values
        new_q0=new_contents[3]
        self.q0=new_contents[3]
        
        
        # Store final states values
        self.f= new_contents[4]

        self.wordVal = False
        self.path=[]
        
        # Validation again
        self.is_valid(self.q, self.sigma, self.delta, self.q0, self.f)
        
        
    def get_empty (self, delta):
        '''
        this method receives all the values of delta to do a processing and recognice the empty value of the file
        
        PARAMETERS: 
        - delta: A dictionary type with the transitions
        
        OUTPUT:
        It returns if the word is lamba or epsilon.
        '''
        
        for key in delta.keys():
            if 'lambda' in key:
                return 'lambda'
                    
            if 'epsilon' in key:
                return 'epsilon'
                
                
    def process_symbol (self, state, symbol):
        '''
        This method receives a string representing the state and the symbol to process and returns 
        the set of possible next states
        
        INPUTS
        - sate: string which indicates the state from which you are testing.
        - symbol: string which indicates the symbol to test along with the state to see the possible outputs
        
        OUPUT
        - Possible next states, it's a list of all the possible results by testing the state and symbol in the automata's 
        configuration.
        '''
        
        entry= state + '-' + symbol
        
        try:
            return self.delta[entry]
        except:
            return []#print("Your state doesn't have state transition")
    
    def empty_transition(self, state):
        '''
        This method receives a string representing the state and returns the set of possible states reachable using empty transitions from that state
        
        PARAMETERS:
        - state: a string representing the states in which the user wants to get the set of possible states reachable using empty transitions.
        
        
        OUTPUT:
        - aceptados: LIST of all states posibles to be reached from the state given in the parameters.
        '''
        
        entry= state + '-' + self.empty
        try:
            return self.delta[entry]
        except:
            return [] #print("Your state doesn't have an empty transition")

    def process_word(self, word):
        '''
        this method receives a string and return a boolean indicating if the string is accepted or not. 
        It also should print the transitions chosen to reach a final state in case that the word is accepted
        
        PARAMETERS:
        - word: STRING to be processed and verified to know if the word is accepted in the automaton or not
        
        OTPUT:
        - raise Exception('Not a valid word'): if the word is given in another data type
        - If the word is accepted, list of states. 
        '''
        
        #translate word
        #lambda filter
        if self.empty in word:
            t = ',' + self.empty + ','
            word = word.replace(self.empty, t)
        #ungrading all
        word = word.split(sep=',')
        aux = []
        for txt in word: # splitting cases in text
            if txt != self.empty:# if it is not lambda
                for key in txt:
                    aux.append(key)
            else:
                aux.append(txt)
        word = aux
        
        #check in the grammar
        self.grammar_processor(word)

        #returning results
        if self.wordVal:
            self.wordVal = False
            self.current = self.q0
            self.path = []
            print(self.lastpath) #
            self.lastpath = []
            return True
        else:
            self.wordVal = False
            self.current = self.q0
            print(self.lastpath)
            self.path = []
            self.lastpath = []
            return False


    def grammar_processor(self, word): ## 010
        ''' Auxiliar function to processing_word function'''
        # recursion
        #when the word reach 1 len
        if len(word) == 1:
            
            # check if one of the av_states is a final state
            #####check empty
            if word[0] == self.empty:
                av_states = self.empty_transition(self.current)
            else:
                av_states = self.process_symbol(self.current, word[0]) ##Returns the possibles casses to a movement
            
            
            ##print(av_states, self.f) ##disponible y que esfinal 
            
            for state in av_states:  ##if my list has q.final
                for end in self.f:
                    if state == end:
                        #print(self.current, '-',word,'-',state)
                        self.path.append([self.current,'-',word[0],'-',state])
                        self.lastpath = self.path.copy()
                        self.wordVal = True
                        break

        #when there is more iterations
        else:
            key = self.current + '-' + word[0] ##Creatimg a new key, for example q1-0

            if word[0] == self.empty:
                av_states = self.empty_transition(self.current)
            else:
                av_states = self.process_symbol(self.current, word[0])
                
            
            ##print( '\nThe list is:', word, 'with', key,'transition to',self.current, '\n')
            for state in av_states:
                self.path.append([self.current,'-',word[0],'-',state])
                #print(self.current,'-',word[0],'-',state)
                self.current = state
                
                self.grammar_processor(word[1:])
                #self.path.pop(-1)
        
                
    def is_valid(self, q, sigma, delta, q0, f):
        '''
        This method receives the automaton elements and returns a boolean indicating if it is a valid automaton definition. Here we decided to implement the SELF because is easier to manage -
        
        Parameters:
        - Self: We used self to access to the elements of the automata. 
        
        Output:
        Boolean with True if the configuration is valid or NotValidConfigurationError if it is not. 
        '''
       # If estado inicial has more than 1 element is not valid
    
        data_type= type(q0) # q0 is a string and it was complicated to do a for-loop.
        
        # We decided to take it as a list, it doesn't really matter how the user types it into the input because we've made sure to cast the string with previous functions.

        if data_type== str:
            pass
        
        #Many elements, the automata only needs 1 initial state q0
        elif (data_type== list): #Counting the elements of the list to discard
            if len(q0)>1:
                raise Exception('NotValidConfigurationError')

            else: 
                self.q0= q0[0]
                self.current = self.q0
        
      # inicial is in Q#
        
        # q0 is not defined in Q states
        if self.q0 not in q: 
            raise Exception('NotValidConfigurationError')
        
        for final in f:
            if final not in q:
                raise Exception('NotValidConfigurationError')

      #### f is not defined in Q states####

        for element in q:
            if element not in q:
                raise Exception('NotValidConfigurationError')

      ##The transitions from state to state are not well defined because they are not found in Q
        d_values= delta.values() ##Accessing delta values, for example q0, 0: q1. Values() is taking only q1
        status= True
        

        aux_list= []
        
        for values in delta.values():
            for states in values:
                aux_list.append(states)
        ##print (delta.values(),aux_list, q)      
        if not (all(x in q for x in list(aux_list))): raise Exception("NotValidConfigurationError")

     ##The alphabet used for the delta transitions is in sigma
        
        for state in delta.keys():
            d_split= state.split(sep= '-')  # In delta.keys() you get, for example, 'q0-1', so a split has been done to get only 1.
            if d_split[0] not in q: ##checking if delta.key [0] is in q
                raise Exception("NotValidConfigurationError")
                
            if d_split[1] not in sigma: ##Checking if it is in sigma
                ##print(d_split[1], self.sigma)
                if d_split[1] not in self.empty:
                    raise Exception("NotValidConfigurationError")

        return True

    def __str__(self):
        '''
        prints current automaton configuration (including current state)

        PARAMETERS:
        - self: To pass the elements of the atomaton'''
        
        return ("Q = {0}\nΣ = {1}\nδ = {2}\nq0 = {3}\nEmpty symbol = {4}\nf = {5}\nCurrent state at {6}".format(self.q, self.sigma, self.delta, self.q0, self.empty, self.f, self.current))