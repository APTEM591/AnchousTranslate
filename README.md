# AnchousTranslate
A simple telegram bot written on c++ for translating things from one language to another.

Uses Google Translation API to automatically translate anything you type. Limit is 1000 characters per hour due to API limits.

Required TG bot token and Google cloud API key to get this bot to work.

## Usage examples:

### input: "/translate uk Hello World!"
```
en: Hello World!
-------------
uk: Привіт Світ!
```

### input: "/translate fr Hello World!"
```
en: Hello World!
-------------
fr: Bonjour le monde!
```

### input: "/limit"
```
You have 395 characters left for this hour
```
