import sys
from greeting_response import GreetingResponse
from introduction_request import IntroductionRequest

def greeting(req: IntroductionRequest) -> GreetingResponse:
    print("Hello, this is my script!")
    message = f"Hello {req.name}, you are {req.age} years old and your interests are {', '.join(req.interests)}."
    return GreetingResponse(message)

if __name__ == "__main__":
    # This block ensures the script can be imported as a module
    sys.path.append('.')
    greeting_request = IntroductionRequest(name="Alice", age=30, interests=["reading", "hiking"])
    response = greeting(greeting_request) 
    print(response.respond())