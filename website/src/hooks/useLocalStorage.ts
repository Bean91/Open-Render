import { useState, useEffect } from "react";

export function useLocalStorage<T>(key: string, initialValue: T) {
    const [value, setValue] = useState<T>(() => {
        try {
            const json = localStorage.getItem(key);
            return json ? JSON.parse(json) : initialValue;
        } catch {
            return initialValue;
        }
    });

    useEffect(() => {
        try {
            localStorage.setItem(key, JSON.stringify(value));
        } catch {
            //Didn't find anything
        }
    }, [value]);

    return [value, setValue] as const;
}