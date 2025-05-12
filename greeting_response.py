class GreetingResponse:
    def __init__(self, greeting,age):
        self.greeting = greeting
        self.age = age

    def respond(self):
        return f"Hello! You said: {self.greeting}"