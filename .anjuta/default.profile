<?xml version="1.0"?>
<anjuta>
    <plugin name="Terminal" mandatory="no">
        <require group="Anjuta Plugin"
                 attribute="Location"
                 value="anjuta-terminal:TerminalPlugin"/>
    </plugin>
    <plugin name="Macro Plugin" mandatory="no">
        <require group="Anjuta Plugin"
                 attribute="Location"
                 value="anjuta-macro:MacroPlugin"/>
    </plugin>
    <plugin name="Debugger" mandatory="no">
        <require group="Anjuta Plugin"
                 attribute="Location"
                 value="anjuta-debug-manager:DebugManagerPlugin"/>
    </plugin>
</anjuta>
