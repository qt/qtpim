function detailTypeToString(detailType) {
    switch (detailType) {
    case ContactDetail.Address:
        return "Address";
    case ContactDetail.Anniversary:
        return "Anniversary";
    case ContactDetail.Avatar:
        return "Avatar";
    case ContactDetail.Birthday:
        return "Birthday";
    case ContactDetail.DisplayLabel:
        return "DisplayLabel";
    case ContactDetail.Email:
        return "Email";
    case ContactDetail.ExtendedDetail:
        return "ExtDetail";
    case ContactDetail.Family:
        return "Family";
    case ContactDetail.Favorite:
        return "Favorite";
    case ContactDetail.Gender:
        return "Gender";
    case ContactDetail.Geolocation:
        return "GeoLocation";
    case ContactDetail.GlobalPresence:
        return "GlobalPresence";
    case ContactDetail.Guid:
        return "Guid";
    case ContactDetail.Hobby:
        return "Hobby";
    case ContactDetail.Name:
        return "Name";
    case ContactDetail.NickName:
        return "NickName";
    case ContactDetail.Note:
        return "Note";
    case ContactDetail.OnlineAccount:
        return "OnlineAccount";
    case ContactDetail.Organization:
        return "Organization";
    case ContactDetail.PersonId:
        return "PersonId";
    case ContactDetail.PhoneNumber:
        return "PhoneNumber";
    case ContactDetail.Presence:
        return "Presence";
    case ContactDetail.Ringtone:
        return "Ringtone";
    case ContactDetail.SyncTarget:
        return "SyncTarget";
    case ContactDetail.Tag:
        return "Tag";
    case ContactDetail.Timestamp:
        return "Timestamp";
    case ContactDetail.Type:
        return "Type";
    case ContactDetail.Url:
        return "Url";
    case ContactDetail.Version:
        return "Version";
    default:
        console.log("UNKNOWN TYPE: " + detailType)
        return "Unknown"
    }
}
