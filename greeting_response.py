class GreetingResponse:
    def __init__(self, greeting):
        self.greeting = greeting

    def respond(self):
        return f"Hello! You said: {self.greeting}"