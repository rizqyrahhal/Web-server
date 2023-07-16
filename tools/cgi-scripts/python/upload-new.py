#!/usr/bin/env python

import sys
import json

def generate_error_response(error_message):
    response = {
        "success": False,
        "error_message": error_message
    }
    return json.dumps(response)

def process_form_data(form_data):
    try:
        # Process the form data here

        # Example: Accessing a specific field named 'bodysfile'
        fileitem = form_data['/Users/rarahhal/Desktop/mankhdmoch7na/bodysfile']

        # Other processing logic...

        # If everything is successful, return a success response
        response = {
            "success": True,
            "message": "Form data processed successfully"
        }
        return json.dumps(response)

    except Exception as e:
        # If an error occurs, generate an error response
        error_message = str(e)
        return generate_error_response(error_message)

def main():
    # Assume form_data is obtained from CGI request

    # Example: form_data is a dictionary containing form fields
    form_data = {
        '/Users/rarahhal/Desktop/mankhdmoch7na/bodysfile': 'example.txt',
        # Other form fields...
    }

    # Process the form data and generate the response
    response = process_form_data(form_data)

    # Set the proper Content-Type header
    print("Content-Type: application/json\r\n")

    # Print the response
    print(response)

if __name__ == "__main__":
    main()