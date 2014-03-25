var env = require('utils.jjv');
env.addSchema('user', {
    type: 'object',
    properties: {
        firstname: {
            type: 'string',
            minLength: 2,
            maxLength: 15
        },
        lastname: {
            type: 'string',
            minLength: 2,
            maxLength: 25
        },
        gender: {
            type: 'string',
            enumerator: ['male', 'female']
        },
        email: {
            type: 'string',
            format: 'email'
        },
        password: {
            type: 'string',
            minLength: 8
        }
    },
    required: ['firstname', 'lastname', 'email', 'password']
});

// Perform validation against an incomplete user object (errors will be reported)
var errors = env.validate('user', {firstname: 'John', lastname: 'Smith', email: 'on.liamg@gmail.com', password: 'Secret10000'});

// validation was successful
if (!errors) {
    print('User has been validated.');
} else {
    print('Failed with error object ' + JSON.stringify(errors));
}
