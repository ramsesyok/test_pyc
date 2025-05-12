class IntroductionRequest:
    def __init__(self, name: str, age: int, interests: list):
        self.name = name
        self.age = age
        self.interests = interests

    def __str__(self):
        return f"Name: {self.name}, Age: {self.age}, Interests: {', '.join(self.interests)}"