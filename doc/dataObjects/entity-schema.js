{
	"type":"object",
	"$schema": "http://json-schema.org/draft-03/schema",
	"id": "http://jsonschema.net",
	"required":false,
	"properties":{
		"description": {
			"type":"string",
			"id": "http://jsonschema.net/description",
			"required":false
		},
		"dimensions": {
			"type":"array",
			"id": "http://jsonschema.net/dimensions",
			"required":false,
			"items":
				{
					"type":"object",
					"id": "http://jsonschema.net/dimensions/0",
					"required":false,
					"properties":{
						"descriptions": {
							"type":"string",
							"id": "http://jsonschema.net/dimensions/0/descriptions",
							"required":false
						},
						"name": {
							"type":"string",
							"id": "http://jsonschema.net/dimensions/0/name",
							"required":false
						}
					}
				}
			

		},
		"enumerators": {
			"type":"array",
			"id": "http://jsonschema.net/enumerators",
			"required":false,
			"items":[
				{
					"type":"object",
					"id": "http://jsonschema.net/enumerators/0",
					"required":false,
					"properties":{
						"name": {
							"type":"string",
							"id": "http://jsonschema.net/enumerators/0/name",
							"required":false
						},
						"values": {
							"type":"array",
							"id": "http://jsonschema.net/enumerators/0/values",
							"required":false,
							"items":
								{
									"type":"string",
									"id": "http://jsonschema.net/enumerators/0/values/0",
									"required":false
								}
							

						}
					}
				},
				{
					"type":"object",
					"id": "http://jsonschema.net/enumerators/1",
					"required":false,
					"properties":{
						"description": {
							"type":"string",
							"id": "http://jsonschema.net/enumerators/1/description",
							"required":false
						},
						"name": {
							"type":"string",
							"id": "http://jsonschema.net/enumerators/1/name",
							"required":false
						},
						"values": {
							"type":"array",
							"id": "http://jsonschema.net/enumerators/1/values",
							"required":false,
							"items":
								{
									"type":"string",
									"id": "http://jsonschema.net/enumerators/1/values/0",
									"required":false
								}
							

						}
					}
				}
			]
		},
		"name": {
			"type":"string",
			"id": "http://jsonschema.net/name",
			"required":false
		},
		"properties": {
			"type":"array",
			"id": "http://jsonschema.net/properties",
			"required":false,
			"items":
				{
					"type":"object",
					"id": "http://jsonschema.net/properties/0",
					"required":false,
					"properties":{
						"description": {
							"type":"string",
							"id": "http://jsonschema.net/properties/0/description",
							"required":false
						},
						"dim": {
							"type":"string",
							"id": "http://jsonschema.net/properties/0/dim",
							"required":false
						},
						"name": {
							"type":"string",
							"id": "http://jsonschema.net/properties/0/name",
							"required":false
						},
						"type": {
							"type":"string",
							"id": "http://jsonschema.net/properties/0/type",
							"required":false
						},
						"unit": {
							"type":"string",
							"id": "http://jsonschema.net/properties/0/unit",
							"required":false
						}
					}
				}
			

		},
		"version": {
			"type":"string",
			"id": "http://jsonschema.net/version",
			"required":false
		}
	}
}
