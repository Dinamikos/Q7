class pushdown_automaton:
    '''
    Push down automaton is a class which accept a tuple of elements
    to create an NDFA rules based in a queue of read write strategy

    (q, Σ, Γ, δ, q0, F) and instantiate the properties in the
    PushdownAutomaton object
    '''
    def initialize(self, q, sigma, gamma, delta, q0, f, empty = 'epsilon'):
        '''
        Parameters:
            q: list of states
            sigma: list of symbols
            gamma: list of symbols for the queue
            delta: dictionary of transitions
            q0: initial state
            f: list of final states
            empty: symbol for empty string
        '''
        self.empty = self.get_empty_symbol(delta)
        # call if it is valid
        self.is_valid(q, sigma, gamma, delta, q0, f)

        self.q = q
        self.sigma = sigma
        self.gamma = gamma
        self.delta = delta
        self.q0 = q0
        self.f = f

        self.word = ''
        self.is_accepted = False
        self.current_state = q0
        self.queue = []
        self.transitions = []
        self.true_transitions = []
        self.count = 0
        self.lastpop = ''
        
    def get_empty_symbol(self, delta):
        '''
        this method receives a delta and returns the empty symbol
        '''
        for transition in delta:
            q, symbol, outo, next, into = self.get_transition_components(transition)
            if symbol == 'epsilon':
                return 'epsilon'
            elif symbol == 'lambda':
                return 'lambda'
        return None

    def load_from_file(self, path:str):
        '''
        this method receives the file with the automaton defninition 
        and loads the properties
        '''

        with open(path, 'r') as file:
            automaton_props = file.read().splitlines()
            automaton_props = [x.replace('{','').replace('}','').replace(' ','').split(sep=',') for x in automaton_props]
        
        # set q prop
        q = automaton_props[0]
        # set sigma prop
        sigma = automaton_props[1]
        # set gamma prop
        gamma = automaton_props[2]
        # set delta prop
        delta = automaton_props[3]
        # set q0 prop
        q0 = automaton_props[4][0]
        # set f prop
        f = automaton_props[5]

        self.initialize(q, sigma, gamma, delta, q0, f)

    def process_symbol(self, state, symbol):
        '''
        this method receives a string representing the state and the 
        symbol to process and returns the set of possible next states
        '''
        transitions = []

        for transition in self.delta:
            q, current, outo, next, into = self.get_transition_components(transition)
            if q == state and current == symbol:
                transitions.append(transition)

        return transitions

    def empty_transition(self, state):
        '''
        this method receives a string representing the state and 
        returns the set of possible states reachable using empty 
        transitions from that state
        '''
        transitions = []

        for transition in self.delta:
            q, symbol, outo, next, into = self.get_transition_components(transition)
            if symbol == self.empty and q == state:
                transitions.append(transition)

        return transitions
    def get_transition_components(self, trasition):
        '''
        this method receives a transition and returns the components
        '''
        return trasition.split(sep='-')

    def process_word(self, word):
        '''
        this method receives a string and return a boolean indicating 
        if the string is accepted or not. It also should print the 
        transitions chosen to reach a final state in case that the
        word is accepted
        '''
        # set word
        self.word = word

        # grammar processing
        # starting state
        self.grammar_processor(word)

        # returning and reseting
        if self.is_accepted:
            print('\n'.join(self.true_transitions))
            self.reset()
            return True
        else:
            self.reset()
            return False

    def reset(self):
        '''
        this method resets the automaton to its initial state
        '''
        self.current_state = self.q0
        self.word = ''
        self.is_accepted = False
        self.queue = []
        self.transitions = []
        self.true_transitions = []
        self.count = 0

    def grammar_processor(self, word):

        if len(word) == 0 and len(self.queue) == 0 and self.current_state in self.f:
            print('accepted with {0}'.format(self.current_state))
            self.is_accepted = True
            self.true_transitions = self.transitions.copy()
            return

        else:
            current_transitions = []
            current_empty_transitions = []
            try:
                current_transitions = self.process_symbol(self.current_state, word[0])
                current_empty_transitions = self.empty_transition(self.current_state)
            except:
                return

            transitions = current_transitions + current_empty_transitions

            for transition in transitions:
                q, symbol, outo, next, into = self.get_transition_components(transition)
                print('transition {0}'.format(transition))
                self.transitions.append(transition)
                self.pop_queue(outo)
                self.put_queue(into)
                self.current_state = next
                if self.count == 0:
                    self.count += 1
                    self.grammar_processor(word)
                else:
                    self.grammar_processor(word[1:])
                    
                    self.pop_queue(into)
                    self.current_state = q
                    self.transitions.pop()
                


    def pop_queue(self, symbol):
        '''
        this method pops the queue
        '''
        print('pop symbol {0} from {1}'.format(symbol, self.queue))
        if symbol != self.empty and len(self.queue) > 0 and symbol == self.queue[-1]:
            self.queue.pop()
    
    def put_queue(self, symbol):
        '''
        this method puts the symbol in the queue
        '''
        if symbol != self.empty:
            self.queue.append(symbol)

    def is_valid(self, q, sigma, gamma, delta, q0, f):
        '''
        this method receives the automaton elements and returns a 
        boolean indicating if it is a valid automaton definition
        '''
        # check if q is a list
        if not isinstance(q, list):
            self.errorHandler('q is not a list')
        # check if sigma is a list
        if not isinstance(sigma, list):
            self.errorHandler('sigma is not a list')
        # check if gamma is a list
        if not isinstance(gamma, list):
            self.errorHandler('gamma is not a list')
        # check if delta is a list
        if not isinstance(delta, list):
            self.errorHandler('delta is not a list')
        # check if q0 is a string
        if not isinstance(q0, str):
            self.errorHandler('q0 is not a string')
        # check if f is a list
        if not isinstance(f, list):
            self.errorHandler('f is not a list')
        # check if q0 is a state
        if not q0 in q:
            self.errorHandler('q0 is not a state')
        # check if f is a subset of q
        if not set(f).issubset(set(q)):
            self.errorHandler('f is not a subset of q')
        # check if delta is a valid transition
        for transition in delta:
            current_q, current, outo, next, into = self.get_transition_components(transition)
            # check if q is a state
            if not current_q in q:
                self.errorHandler('current_q is not a state of q')
            # check if current is a symbol
            if not current in sigma:
                if current != self.empty:
                    self.errorHandler('current is not a symbol')
            # check if outo is a symbol
            if not outo in gamma:
                if outo != self.empty:
                    self.errorHandler('outo is not a symbol')
            # check if next is a state
            if not next in q:
                self.errorHandler('next {0} is not a state of {1}'.format(next, q))
            # check if into is a symbol
            if not into in gamma:
                if into != self.empty:
                    self.errorHandler('{0} is not a symbol in {1}'.format(into, gamma))

    def errorHandler(self, error):
        '''
        this method receives an error and prints it
        '''
        raise Exception('NotValidConfigurationError:\n check your configuration according with: {0}'.format(error))

    def __str__(self):
        '''
        prints current automaton configuration
        '''
        return ("Q = {0}\nΣ = {1}\nΓ = {2}\nδ = {3}\nq0 = {4}\nF = {5}\nCurrent state at {6}".format(self.q, self.sigma, self.gamma, self.delta, self.q0, self.f, self.current_state))


automaton = pushdown_automaton()
automaton.load_from_file('automaton2.txt')
# print(automaton)
# print('\n' )
# print(automaton.process_symbol('q2', '0'))

print(automaton.process_word('01101'))
    